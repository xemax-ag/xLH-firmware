export function Terminal() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      {/*<h1 className="text-xl font-bold">Terminal</h1>*/}
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{minHeight: 600, width: "100%"}}
        src="/terminal/"
        title="Terminal"
        sandbox="allow-scripts allow-same-origin allow-forms allow-popups"
      />

      <h2 className="text-lg font-bold">The most important commands</h2>

      <table className="table-auto">
        <thead>
        <tr className="text-left">
          <th>Command</th>
          <th>Description</th>
        </tr>
        </thead>
        <tbody>
        <tr>
          <td className="font-bold">make config</td>
          <td>Calling up the configuration environment (WiFi, localication, ...). Navigation is done using the arrow
            keys, tab key and return key.
          </td>
        </tr>
        <tr>
          <td className="font-bold">make netconfig</td>
          <td>Calling up the network configuration.Navigation is done using the arrow keys, tab key and return key.</td>
        </tr>
        <tr>
          <td className="font-bold">make reboot</td>
          <td>Restarting the computer</td>
        </tr>
        <tr>
          <td className="font-bold">make shutdown</td>
          <td>Turning off the computer</td>
        </tr>
        </tbody>
      </table>

      <p>
        The xLH computers are based on a Linux Debian operating system. Further information can be found in the
        official <a href="https://www.debian.org/doc/manuals/debian-reference/" target="_blank">documentation</a>.
      </p>
    </div>
  )
}
