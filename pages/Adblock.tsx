const AdBlock = props => {
  const { userLoaded, currentUserIsSilverPlus } = useAuth()

  useEffect(() => {
    window.adsbygoogle = window.adsbygoogle || []
    window.adsbygoogle.push({})
  }, [])

  const reloadAds = () => {
    const { googletag } = window
    if (googletag) {
      googletag.cmd.push(function () {
        googletag.pubads().refresh()
      })
    }
  }

  Router.events.on("routeChangeStart", reloadAds)

  if (userLoaded && !currentUserIsSilverPlus) {
    return (
      <ins
        className="adsbygoogle adbanner-customize"
        style={{ display: "block" }}
        data-ad-client="ca-pub-1702181491157560"
        data-ad-slot={props.slot}
        data-ad-format={props.format}
        data-full-width-responsive={props.responsive}
      />
    )
  }
  return ""
}

export default AdBlock
