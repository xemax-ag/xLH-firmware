export function ApiDocs() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-320 mx-auto px-4">
      {/*<h1 className="text-xl font-bold">API-docs</h1>*/}
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{ minHeight: 750, width: "100%" }}
        src="/docs"
        title="Docs"
      />
    </div>
  )
}
