import {useEffect} from "react"
import {
  Route,
  Routes,
  unstable_HistoryRouter as HistoryRouter,
} from "react-router-dom"

import {Header} from "@/components/header"
import {browserHistory} from "@/lib/history"
import {Home} from "@/pages/home.tsx"
import {Terminal} from "@/pages/terminal.tsx"
import {Help} from "@/pages/help.tsx";
import {PLCVisualization} from "@/pages/plc_visualization.tsx";
import {ApiDocs} from "@/pages/api_docs.tsx";
import {Jupyterlite} from "@/pages/jupyterlite.tsx";

function isEditableTarget(target: EventTarget | null) {
  if (!(target instanceof HTMLElement)) return false
  if (target.isContentEditable) return true
  return (
    target.closest("input, textarea, select, [contenteditable='true']") !==
    null
  )
}

export function App() {
  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      if (event.repeat) return
      if (event.metaKey || event.ctrlKey || event.altKey) return
      if (isEditableTarget(event.target)) return

      const key = event.key.toLowerCase()
      const path =
        key === "t" ? "/terminal/" :
          key === "p" ? "/plc/" :
            key === "v" ? "/plc/" :
              key === "j" ? "/jupyterlite/lab/" :
                key === "d" ? "/docs" :
                  null
      if (!path) return

      const base = import.meta.env.DEV
        ? import.meta.env.API_BASE_URL
        : window.location.origin
      const url = `${base}${path}`
      const win = window.open(url, "_blank")
      win?.focus()
    }

    window.addEventListener("keydown", handleKeyDown)
    return () => window.removeEventListener("keydown", handleKeyDown)
  }, [])

  return (
    <HistoryRouter history={browserHistory}>
      <div className="flex h-full min-h-svh w-full flex-col">
        <Header/>
        <div className="flex flex-1 p-6">
          <div className="flex min-w-0 flex-1 flex-col gap-4 text-sm leading-loose">
            <Routes>
              <Route path="/" element={<Home/>}/>
              <Route path="/ui-plc" element={<PLCVisualization/>}/>
              <Route path="/ui-terminal" element={<Terminal/>}/>
              <Route path="/ui-jupyterlite" element={<Jupyterlite/>}/>
              <Route path="/ui-docs" element={<ApiDocs/>}/>
              <Route path="/help" element={<Help/>}/>
            </Routes>
          </div>
        </div>
      </div>
    </HistoryRouter>
  )
}

export default App

// https://medium.com/@ivantsov/using-react-router-and-history-38c021270829