class JsonCacheManager<Class T> {
    private string m_JsonFilePath;

    protected ref T m_MarkersCache;

    void Init(string jsonFilePath, T markersCache) {
        m_JsonFilePath = jsonFilePath;
        m_MarkersCache = markersCache;
        LoadCache();
    }

    void LoadCache() {
        if (FileExist(m_JsonFilePath)) {
            JsonFileLoader<T>.JsonLoadFile(m_JsonFilePath, m_MarkersCache);
        } else {
            LoadDefaults();
        }
    }

    void SaveCache() {
        JsonFileLoader<T>.JsonSaveFile(m_JsonFilePath, m_MarkersCache);
    }

    /*
    	Returns the big array
    */
    ref array<ref MarkerInfo> GetCachedArray(string addressPort = "") {
        return m_MarkersCache.GetCachedArray(addressPort);
    }

    void LoadDefaults();
};