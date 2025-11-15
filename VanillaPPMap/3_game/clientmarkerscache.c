class ClientMarkersCache extends JsonCacheManager<MapMarkersCache> {
    void ClientMarkersCache() {
        Init("$profile:MapMarkersCache.json", new MapMarkersCache);
    }


    override void LoadDefaults() {
        // no defaults
    }

    /*
    	Update the markers cache array
    	@Param 1: markers array
    */
    void UpdateCachedMarkerInfos(ref array<ref MarkerInfo> updatedArray, string addressPort = "") {
        m_MarkersCache.SetCachedArray(updatedArray, addressPort);
    }

    /*
      Remove Marker by its position
    */
    void RemoveCachedMarker(vector pos, string addressPort = "") {
        ref array<ref MarkerInfo> markers = m_MarkersCache.GetCachedArray(addressPort);
        if (markers) {
            foreach(MarkerInfo marker : markers) {
                if (marker && marker.GetPosition() == pos) {
                    m_MarkersCache.RemoveCachedMarkerInfo(marker, addressPort);
                }
            }
        }
    }
};
