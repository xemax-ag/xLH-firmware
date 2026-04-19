import {ExternalLink} from "lucide-react"
import {Button} from "@/components/ui/button"

export function Home() {
  return (
    <div className="flex flex-col gap-4 w-full max-w-content mx-auto px-4">
      <h1 className="text-xl font-bold">Home</h1>


      <div className="flex flex-row">
        <div className="basis-32">
          <Button className="text-regal-blue" variant="outline" size="lg">
            <ExternalLink data-icon="inline-start"/>
            Button
          </Button>
        </div>
        <div className="basis-128 bg-red-">03</div>
      </div>

    </div>
  )
}
