import {useEffect} from "react"
import {ExternalLink} from "lucide-react"
import {useQuery} from "@tanstack/react-query"
import {Button} from "@/components/ui/button"
import {
  Accordion,
  AccordionContent,
  AccordionItem,
  AccordionTrigger,
} from "@/components/ui/accordion"
import {useAppDispatch, useAppSelector} from "@/store"
import {setOpenAccordionItem} from "@/store/home-slice"
import type {components} from "@/apis/xlh_api.ts"

type AppVersion = components["schemas"]["AppVersion"]

function openExternal(url: string) {
  const win = window.open(`${url}`, "_blank")
  win?.focus()
}

async function fetchAppVersion(): Promise<AppVersion> {
  const base = import.meta.env.DEV
    ? import.meta.env.API_BASE_URL
    : window.location.origin
  const res = await fetch(`${base}/version/app/`)
  if (!res.ok) throw new Error(`version/app failed: ${res.status}`)
  return res.json()
}

export function Home() {
  const openItem = useAppSelector((s) => s.home.openAccordionItem)
  const dispatch = useAppDispatch()
  const {data: appVersion, refetch: refetchAppVersion} = useQuery({
    queryKey: ["/version/app/"],
    queryFn: fetchAppVersion,
    enabled: false,
  })

  useEffect(() => {
    // console.log(`${import.meta.env.API_BASE_URL}/version/app/`)
    refetchAppVersion()
  }, [refetchAppVersion])

  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      <h1 className="text-xl font-bold">xLH xemax Learning Hub</h1>
      <p className="">Version {appVersion?.version ?? "…"}</p>
      <Accordion
        type="single"
        collapsible
        value={openItem}
        onValueChange={(v) => dispatch(setOpenAccordionItem(v))}
        className="max-w-content"
      >
        <AccordionItem value="plc">
          <AccordionTrigger className="font-bold">PLC programmable logic controller</AccordionTrigger>
          <AccordionContent>
            <div className="flex-1">
              <div className="flex flex-row items-center pt-2 pb-2">
                <div className="basis-10">
                  <Button variant="default" size="icon" onClick={() => openExternal("/plc/")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  PLC-Visualization
                </div>
              </div>

              <div className="flex flex-row items-center pb-2">
                <div className="basis-10">
                  <Button variant="default" size="icon" onClick={() => openExternal("https://xlh.xemax.ch/")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  xLH Documentation
                </div>
              </div>

              <div className="flex flex-row items-center pb-2">
                <div className="basis-10">
                  <Button variant="default" size="icon" onClick={() => openExternal("https://www.helpme-codesys.com/")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  CODESYS documentation
                </div>
              </div>
            </div>
          </AccordionContent>
        </AccordionItem>

        <AccordionItem value="jupyterlite">
          <AccordionTrigger className="font-bold">Python - Jupyterlite development environment</AccordionTrigger>
          <AccordionContent>
            <div className="flex-1">
              <div className="flex flex-row items-center pt-2 pb-2">
                <div className="basis-10">
                  <Button variant="default" size="icon" onClick={() => openExternal("/jupyterlite/lab/")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  Jupyterlite Python <a href="https://webassembly.org/" target="_blank">Wasm</a> environment
                </div>
              </div>

              <div className="flex flex-row items-center pb-2">
                <div className="basis-10">
                  <Button variant="default" size="icon"
                          onClick={() => openExternal("/jupyterlite/lab/?fromURL=https://raw.githubusercontent.com/xemax-ag/xLH-projects/refs/heads/main/xlh-rgb/notebooks/rgb_example.ipynb")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  Jupyterlite example: rgb_example.ipynb
                </div>
              </div>

              <div className="flex flex-row items-center pb-2">
                <div className="basis-10">
                  <Button variant="default" size="icon"
                          onClick={() => openExternal("/jupyterlite/lab/?fromURL=https://raw.githubusercontent.com/xemax-ag/xLH-projects/refs/heads/main/xlh-rgb/notebooks/math_example.ipynb")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  Jupyterlite example: math_example.ipynb
                </div>
              </div>

              <div className="flex flex-row items-center">
                <div className="basis-10">
                  <Button variant="default" size="icon"
                          onClick={() => openExternal("https://jupyterlite.readthedocs.io/en/stable/")}>
                    <ExternalLink data-icon="inline-start"/>
                  </Button>
                </div>
                <div className="flex-1">
                  Jupyterlite documentation
                </div>
              </div>
            </div>
          </AccordionContent>
        </AccordionItem>
      </Accordion>


    </div>
  )
}
