export function PLCVisualization() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-250 mx-auto px-4">
      <h1 className="text-xl font-bold">Visualization</h1>
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{ height: 720, width: 1280 }}
        src={`${import.meta.env.TERMINAL_BASE_URL}/plc/`}
        title="Terminal"
      />
    </div>
  )
}
