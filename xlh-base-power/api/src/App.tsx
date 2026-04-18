import { useEffect } from "react"
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
      if (event.key.toLowerCase() !== "t") return

      const url = `${import.meta.env.TERMINAL_BASE_URL}/shell/`
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
              <Route path="/visualization" element={<PLCVisualization />}/>
              <Route path="/terminal" element={<Terminal />}/>
              {/*<Route path="/do" element={<Home/>}/>*/}
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