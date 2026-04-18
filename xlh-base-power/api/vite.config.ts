import path from "path"
import { readFileSync } from "fs"
import tailwindcss from "@tailwindcss/vite"
import react from "@vitejs/plugin-react"
import { defineConfig } from "vite"

// Load env: dev uses nextecmedia_ui_dev.env, production uses nextecmedia_ui_prod.env
const envFileName = process.env.NODE_ENV === "production" ? ".dev.env" : ".prod.env"
const envFile = readFileSync(path.resolve(__dirname, `src/${envFileName}`), "utf-8")
const envVars: Record<string, string> = {}
for (const line of envFile.split("\n")) {
  const trimmed = line.trim()
  if (!trimmed || trimmed.startsWith("#")) continue
  const [key, ...rest] = trimmed.split("=")
  envVars[`import.meta.env.${key}`] = JSON.stringify(rest.join("="))
}

// https://vite.dev/config/
export default defineConfig({
  plugins: [react(), tailwindcss()],
  define: envVars,
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src"),
    },
  },
})
