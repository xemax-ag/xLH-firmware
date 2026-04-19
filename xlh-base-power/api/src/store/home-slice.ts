import { createSlice, type PayloadAction } from "@reduxjs/toolkit"

type HomeState = {
  openAccordionItem: string
}

export const HOME_STORAGE_KEY = "home-state"

function loadInitialState(): HomeState {
  try {
    const raw = localStorage.getItem(HOME_STORAGE_KEY)
    if (raw) {
      const parsed = JSON.parse(raw) as Partial<HomeState>
      if (typeof parsed.openAccordionItem === "string") {
        return { openAccordionItem: parsed.openAccordionItem }
      }
    }
  } catch {
    // fall through to default
  }
  return { openAccordionItem: "" }
}

const initialState: HomeState = loadInitialState()

const homeSlice = createSlice({
  name: "home",
  initialState,
  reducers: {
    setOpenAccordionItem(state, action: PayloadAction<string>) {
      state.openAccordionItem = action.payload
    },
  },
})

export const { setOpenAccordionItem } = homeSlice.actions
export default homeSlice.reducer
