export function Help() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      {/*<h1 className="text-2xl font-bold">Help</h1>*/}
      <p className="px-4">The help page contains information about the features of the website.</p>
      <ul className="list-disc pl-8 space-y-1">
        <li>
          Home - opens the different options, each in a new browser tab.
        </li>
        <li>
          Visualization - interactive display of the CODESYS web visualization.
        </li>
        <li>
          Terminal - shell console. Automatic login with the user xlh.
        </li>
        <li>
          Python - Jupyterlite development environment for writing Python programs. Note
          that the Python interpreter is based on <a href="https://webassembly.org/" target="_blank">Wasm</a>, so
          Python code runs in the browser, not on the xLH machine.
        </li>

        <li>
          Docs - description of the REST API.
        </li>

        <li>Keyboard shortcuts
          <ul className="list-disc pl-8 space-y-1">
            <li>[x]: toggle light and dark mode</li>
            <li>[t]: open the Terminal page in a new browser tab</li>
            <li>[p]: open the PLC Visualization page in a new browser tab</li>
            <li>[j]: open the Python (Jupyterlite) page in a new browser tab</li>
            <li>[d]: open the Docs page in a new browser tab</li>
          </ul>
        </li>
      </ul>
    </div>
  )
}
