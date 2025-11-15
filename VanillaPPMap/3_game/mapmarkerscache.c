class MapMarkersCache extends MarkersCacheInterface {
    ref array<ref MarkerInfo> M_MARKER_CACHE_ARRAY = new array<ref MarkerInfo>;
    ref array<ref CustomServerMarkers> customServerMarkers = new array<ref CustomServerMarkers>;

    override array<ref MarkerInfo> GetCachedArray(string addressPort = "") {
        if (addressPort.Length() <= 0) {
            return M_MARKER_CACHE_ARRAY;
        } else {
            array<ref MarkerInfo> markers;
            foreach (CustomServerMarkers serverMarkers : customServerMarkers) {
                if (serverMarkers.addressPort == addressPort) {
                    markers = serverMarkers.serverMarkers;
                }
            }
            if (!markers) {
                markers = new array<ref MarkerInfo>;
                CustomServerMarkers newServerMarkers = new CustomServerMarkers;
                newServerMarkers.addressPort = addressPort;
                newServerMarkers.serverMarkers = markers;
                customServerMarkers.Insert(newServerMarkers);
            }
            return markers;
        }
    }

    override void SetCachedArray(ref array<ref MarkerInfo> cachedArray, string addressPort = "") {
        if (addressPort.Length() <= 0) {
            M_MARKER_CACHE_ARRAY = cachedArray;
        } else {
            bool serverMarkersExists = false;
            foreach (CustomServerMarkers serverMarkers : customServerMarkers) {
                if (serverMarkers && serverMarkers.addressPort == addressPort) {
                    serverMarkers.serverMarkers = cachedArray;
                    serverMarkersExists = true;
                }
            }
            if (!serverMarkersExists) {
                CustomServerMarkers newServerMarkers = new CustomServerMarkers;
                newServerMarkers.addressPort = addressPort;
                newServerMarkers.serverMarkers = cachedArray;
                customServerMarkers.Insert(newServerMarkers);
            }
        }
    }

    override void RemoveCachedMarkerInfo(ref MarkerInfo marker, string addressPort = "") {
        if (addressPort.Length() <= 0) {
            M_MARKER_CACHE_ARRAY.RemoveItem(marker);
        } else {
            array<ref MarkerInfo> markers = GetCachedArray(addressPort);
            markers.RemoveItem(marker);
            SetCachedArray(markers, addressPort);
        }
    }
};