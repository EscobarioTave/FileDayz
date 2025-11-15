class ServerMarkersCache extends JsonCacheManager<VPPMapConfig> {
    const static string SAVE_PATH = "$profile:VPPMapConfig.json";

    void ServerMarkersCache() {
        Init("$profile:VPPMapConfig.json", new VPPMapConfig);
    }

    override void LoadDefaults() {
        VPPMapConfig.Cast(m_MarkersCache).SetUse3DMarkers(true);
        VPPMapConfig.Cast(m_MarkersCache).SetOwnPositionMarkerDisabled(false);
        VPPMapConfig.Cast(m_MarkersCache).SetForceToUseMapItem(false);
        array<ref MarkerInfo> arr = new array<ref MarkerInfo>;
        arr.Insert( new MarkerInfo("Bohemia Interactive","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa",Vector(255,255,255),Vector(7500,0,7500),true,true));
        arr.Insert( new MarkerInfo("Loot Stash","VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypointeditor_CA.paa",Vector(255,255,255),Vector(7400,0,7200),true,true));
        m_MarkersCache.SetCachedArray(arr);
        SaveCache();
        LoadCache();
    }

    bool CanUse3DMarkers() {
        LoadCache();
        return VPPMapConfig.Cast(m_MarkersCache).CanUse3DMarkers();
    }

    bool OwnPositionMarkerDisabled() {
        LoadCache();
        return VPPMapConfig.Cast(m_MarkersCache).OwnPositionMarkerDisabled();
    }

    bool ForceToUseMapItem(){
        LoadCache();
        return VPPMapConfig.Cast(m_MarkersCache).ForceToUseMapItem();
    }
};