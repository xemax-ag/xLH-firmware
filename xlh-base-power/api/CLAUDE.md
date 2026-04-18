# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Commands

Frontend (from repo root):
- `npm run dev` — Vite dev server
- `npm run build` — `tsc -b && vite build` (output → `dist/`)
- `npm run typecheck` / `npm run lint` / `npm run format`
- `npm run preview`

Backend (Python 3.13, managed with `uv`):
- `./api_dev.sh` — uvicorn with `--reload` on port 8000
- `./api.sh` — single-worker production run
- `uv sync` / `uv lock --upgrade` to manage deps

Makefile wraps common ops on the target device:
- `make dev` / `make build` — frontend shortcuts
- `make restart` / `make log` — systemd control for `xlh_api.service`
- `make restart_sm` / `make log_sm` — `xlh_rgb_shared_memory.service`
- `make ssh_dev` — build + rsync to `$HOST` and restart both services (uses `sshpass`, excludes dev-only files per `ssh_upload` rule)

## Architecture

This is a FastAPI backend that serves a React SPA from the same process.

**Serving model (`app/main.py`):** FastAPI registers routers first, then mounts `SPAStaticFiles` at `/` pointing at `dist/`. The SPA mount serves `index.html` as a 404 fallback so client-side routes work, but explicitly raises 404 for paths under `docs`, `v1/`, and `static/` so those prefixes never fall through to the SPA. `/static` is a separate `StaticFiles` mount for backend assets.

**Hardware-aware routing:** `get_xlh_type()` in `app/core/toolbox/xlh_type_cpu_info.py` reads `/proc/cpuinfo`. Only `XlhType.BASE` devices register the `/v1/rgb` router. Keep this conditional when adding new hardware-specific endpoints.

**RGB service:** `app_rgb/rgb_shared_memory.py` runs as a separate systemd unit (`xlh_rgb_shared_memory.service`) and communicates with the API via shared memory — restarts must cover both services (see `make ssh_dev`).

**Error handling:** The global `StarletteHTTPException` handler branches on whether `v1` is in the path — API routes return JSON, everything else renders Jinja templates from `app/core/templates/`.

## Frontend stack

- **Router:** React Router v7 with `unstable_HistoryRouter` + a shared `browserHistory` from `UNSAFE_createBrowserHistory` (`src/lib/history.ts`). This is deliberate — the standalone `history` package's `BrowserHistory` type is missing `createURL`/`encodeLocation` that v7 requires. Use `browserHistory.push(...)` for imperative navigation outside components.
- **State:** Redux Toolkit store in `src/store/index.ts` (typed `useAppDispatch`/`useAppSelector` hooks exported there). `src/store/app-slice.ts` is a placeholder — configureStore requires at least one reducer or it warns.
- **Data fetching:** TanStack Query (`QueryClientProvider` in `src/main.tsx`) with Devtools in dev.
- **Theming:** `src/components/theme-provider.tsx` manages light/dark/system, persists to localStorage, and listens for the `d` key to toggle globally. `useTheme()` is the public API.
- **UI:** shadcn/ui components live in `src/components/ui/`. Path alias `@` → `src/` (tsconfig + Vite).

## Env injection (important, non-standard)

`vite.config.ts` reads `src/.dev.env` or `src/.prod.env` (chosen via `NODE_ENV`) and injects every `KEY=VALUE` pair into the build through Vite's `define` as `import.meta.env.KEY`. Consequences:
- Variables do **not** need the `VITE_` prefix.
- Values are inlined at build time — restart the dev server after editing env files.
- Reference them directly: `import.meta.env.TERMINAL_BASE_URL`.
