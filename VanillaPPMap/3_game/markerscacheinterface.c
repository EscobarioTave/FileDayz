class MarkersCacheInterface {

    array<ref MarkerInfo> GetCachedArray(string addressPort = "");

    void SetCachedArray(ref array<ref MarkerInfo> cachedArray, string addressPort = "");

    void RemoveCachedMarkerInfo(ref MarkerInfo marker, string addressPort = "");
};