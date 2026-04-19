import { createSlice, type PayloadAction } from "@reduxjs/toolkit"

type HomeState = {
  openAccordionItems: string[]
}

export const HOME_STORAGE_KEY = "home-state"

function loadInitialState(): HomeState {
  try {
    const raw = localStorage.getItem(HOME_STORAGE_KEY)
    if (raw) {
      const parsed = JSON.parse(raw) as Partial<HomeState>
      if (Array.isArray(parsed.openAccordionItems)) {
        return { openAccordionItems: parsed.openAccordionItems }
      }
    }
  } catch {
    // fall through to default
  }
  return { openAccordionItems: ["plc"] }
}

const initialState: HomeState = loadInitialState()

const homeSlice = createSlice({
  name: "home",
  initialState,
  reducers: {
    setOpenAccordionItems(state, action: PayloadAction<string[]>) {
      state.openAccordionItems = action.payload
    },
  },
})

export const { setOpenAccordionItems } = homeSlice.actions
export default homeSlice.reducer
