export function Jupyterlite() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      {/*<h1 className="text-xl font-bold">Jupyterlite</h1>*/}
      <iframe
        className="col-md-12 mb-4 w-full"
        style={{ minHeight: 750, width: "100%" }}
        src={`${import.meta.env.API_BASE_URL}/jupyterlite/lab/`}
        title="Jupyterlite"
      />
    </div>
  )
}
