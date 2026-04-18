import { Navigation } from "@/components/navigation"
import { ThemeToggle } from "@/components/theme-toggle"

export function Header() {
  return (
    <header className="grid grid-cols-3 items-center gap-4 border-b px-6 py-3">
      <div className="flex items-center">
        <a
          href="https://xlh.xemax.ch/"
          target="_blank"
          rel="noreferrer"
          aria-label="xlh.xemax.ch"
          className="flex items-center"
        >
          <img
            src="/images/logo/xemax/logo.svg"
            alt="Xemax"
            className="h-6 w-auto dark:hidden"
          />
          <img
            src="/images/logo/xemax/logo-dark.svg"
            alt="Xemax"
            className="hidden h-6 w-auto dark:block"
          />
        </a>
      </div>
      <div className="flex justify-center">
        <Navigation />
      </div>
      <div className="flex justify-end">
        <ThemeToggle />
      </div>
    </header>
  )
}
