import React from 'react'
import { useObservable} from 'rxjs-hooks'
import { fromEvent } from 'rxjs'
import { map, takeUntil, tap, mergeMap } from 'rxjs/operators'


function App() {
	const onDown$ = fromEvent(document.body, 'mousedown')
	const onUp$ = fromEvent(document.body, 'mouseup')
	const onMove$ = fromEvent(document.body, 'mousemove')

	const state$ = onDown$.pipe(
		mergeMap(down => onMove$.pipe(
			takeUntil(onUp$),
		)),
	).pipe(
		tap(console.log),
		map(e => ({top: e.clientY, left: e.clientX})),
	)
	const state = useObservable(() => state$)

  return (
	<div id="box"
		style={state}
	/>
  )
}
export default App
