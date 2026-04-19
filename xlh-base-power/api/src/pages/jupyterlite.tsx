export function Jupyterlite() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      {/*<h1 className="text-xl font-bold">Jupyterlite</h1>*/}
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{ minHeight: 750, width: "100%" }}
        src="/jupyterlite/lab/"
        title="Jupyterlite"
        sandbox="allow-scripts allow-same-origin allow-forms allow-popups"
      />
    </div>
  )
}
