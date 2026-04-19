import path from "path"
import { readFileSync } from "fs"
import tailwindcss from "@tailwindcss/vite"
import react from "@vitejs/plugin-react"
import { defineConfig } from "vite"

function loadEnvVars(mode: string): Record<string, string> {
  const envFileName = mode === "production" ? ".prod.env" : ".dev.env"
  const envFile = readFileSync(
    path.resolve(__dirname, `src/${envFileName}`),
    "utf-8"
  )
  const envVars: Record<string, string> = {}
  for (const line of envFile.split("\n")) {
    const trimmed = line.trim()
    if (!trimmed || trimmed.startsWith("#")) continue
    const [key, ...rest] = trimmed.split("=")
    envVars[`import.meta.env.${key}`] = JSON.stringify(rest.join("="))
  }
  return envVars
}

// https://vite.dev/config/
export default defineConfig(({ mode }) => ({
  plugins: [react(), tailwindcss()],
  root: path.resolve(__dirname, "src"),
  publicDir: path.resolve(__dirname, "public"),
  build: {
    outDir: path.resolve(__dirname, "dist"),
    emptyOutDir: true,
  },
  define: loadEnvVars(mode),
  resolve: {
    alias: {
      "@": path.resolve(__dirname, "./src"),
    },
  },
}))
