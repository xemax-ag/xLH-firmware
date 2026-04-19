export function PLCVisualization() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      {/*<h1 className="text-xl font-bold">Visualization</h1>*/}
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{ minHeight: 720, minWidth: 1280 }}
        src="/plc/"
        title="Terminal"
      />
    </div>
  )
}
