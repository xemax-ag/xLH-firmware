import { configureStore } from "@reduxjs/toolkit"
import { useDispatch, useSelector } from "react-redux"

import appReducer from "./app-slice"
import homeReducer, { HOME_STORAGE_KEY } from "./home-slice"

export const store = configureStore({
  reducer: {
    app: appReducer,
    home: homeReducer,
  },
})

store.subscribe(() => {
  try {
    localStorage.setItem(
      HOME_STORAGE_KEY,
      JSON.stringify(store.getState().home)
    )
  } catch {
    // ignore quota / private mode errors
  }
})

export type RootState = ReturnType<typeof store.getState>
export type AppDispatch = typeof store.dispatch

export const useAppDispatch = useDispatch.withTypes<AppDispatch>()
export const useAppSelector = useSelector.withTypes<RootState>()
