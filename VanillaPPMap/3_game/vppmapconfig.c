class VPPMapConfig extends MarkersCacheInterface {
    private ref array<ref MarkerInfo> M_STATIC_MARKER_ARRAY = new array<ref MarkerInfo>;
    private bool m_CanUse3DMarkers = true;
    private bool m_OwnPositionMarkerDisabled = false;
    private bool m_ForceMapItemOnly = false;

    override array<ref MarkerInfo> GetCachedArray(string addressPort = "") {
        return M_STATIC_MARKER_ARRAY;
    }

    override void SetCachedArray(ref array<ref MarkerInfo> cachedArray, string addressPort = "") {
        M_STATIC_MARKER_ARRAY = cachedArray;
    }

    bool CanUse3DMarkers() {
        return m_CanUse3DMarkers;
    }

    void SetUse3DMarkers(bool canUse3dMarkers) {
        m_CanUse3DMarkers = canUse3dMarkers;
    }

    bool OwnPositionMarkerDisabled() {
        return m_OwnPositionMarkerDisabled;
    }

    void SetOwnPositionMarkerDisabled(bool ownPositionMarkerDisabled) {
        m_OwnPositionMarkerDisabled = ownPositionMarkerDisabled;
    }

    bool ForceToUseMapItem(){
        return m_ForceMapItemOnly;
    }

    void SetForceToUseMapItem(bool state){
        m_ForceMapItemOnly = state;
    }
};