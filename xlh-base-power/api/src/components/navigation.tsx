import {
  NavigationMenu,
  NavigationMenuItem,
  NavigationMenuLink,
  NavigationMenuList,
  navigationMenuTriggerStyle,
} from "@/components/ui/navigation-menu"

export function Navigation() {
  return (
    <NavigationMenu>
      <NavigationMenuList>

        <NavigationMenuItem>
          <NavigationMenuLink
            href="/"
            className={navigationMenuTriggerStyle()}
          >
            Home
          </NavigationMenuLink>
        </NavigationMenuItem>

        {/*<NavigationMenuItem>*/}
        {/*  <NavigationMenuTrigger>Devices</NavigationMenuTrigger>*/}
        {/*  <NavigationMenuContent>*/}
        {/*    <ul className="grid w-[220px] gap-1 p-2">*/}
        {/*      <li>*/}
        {/*        <NavigationMenuLink href="/devices/rgb">*/}
        {/*          RGB*/}
        {/*        </NavigationMenuLink>*/}
        {/*      </li>*/}
        {/*      <li>*/}
        {/*        <NavigationMenuLink href="/devices/sensors">*/}
        {/*          Sensors*/}
        {/*        </NavigationMenuLink>*/}
        {/*      </li>*/}
        {/*    </ul>*/}
        {/*  </NavigationMenuContent>*/}
        {/*</NavigationMenuItem>*/}


        <NavigationMenuItem>
          <NavigationMenuLink
            href="/visualization"
            className={navigationMenuTriggerStyle()}
          >
            Visualization
          </NavigationMenuLink>
        </NavigationMenuItem>

        <NavigationMenuItem>
          <NavigationMenuLink
            href="/terminal"
            className={navigationMenuTriggerStyle()}
          >
            Terminal
          </NavigationMenuLink>
        </NavigationMenuItem>

        <NavigationMenuItem>
          <NavigationMenuLink
            href="/docs"
            className={navigationMenuTriggerStyle()}
          >
            Docs
          </NavigationMenuLink>
        </NavigationMenuItem>

        <NavigationMenuItem>
          <NavigationMenuLink
            href="/help"
            className={navigationMenuTriggerStyle()}
          >
            Help
          </NavigationMenuLink>
        </NavigationMenuItem>

      </NavigationMenuList>
    </NavigationMenu>
  )
}
