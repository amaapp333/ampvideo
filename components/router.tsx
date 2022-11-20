import { useRouter } from 'next/router'

export default function Page() {
const router = useRouter();
useEffect(() => {
router.events.on('routeChangeError', handleChange)
  }, [])
  return (
    <>
    </>
  )
}
