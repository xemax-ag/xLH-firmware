export function Terminal() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-250 mx-auto px-4">
      <h1 className="text-xl font-bold">Terminal</h1>
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{ height: 600, width: "100%" }}
        src={`${import.meta.env.TERMINAL_BASE_URL}/shell/`}
        title="Terminal"
      />
    </div>
  )
}
