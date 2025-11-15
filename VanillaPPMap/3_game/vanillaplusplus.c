modded class DayZGame {
    /* Global Vars [These are always controled even when player leaves session.] */
    private bool m_IsKeyboardBusy;
    private bool m_CanUse3DMarkers = true;
    private bool m_OwnPositionMarkerDisabled = false;
    private ref ClientMarkersCache m_ClientMarkersCache;
    private ref array<ref VPP3DMarker> m_TempHidden3dMarkers;
    private ref array<ref VPP3DMarker> m_Client3dMarkers;
    private ref array<ref VPP3DMarker> m_Server3dMarkers; //Static 3Dmarkers we get from server upon connect.
    private ref array<ref MarkerInfo> m_ServerMarkers; //Static markers we get from server upon connect.

    private bool m_Use3dMarkersSet;
    private bool m_ServerMarkersSet;
    private bool m_ForcePhysicalMap;

    void DayZGame() {
        Print("DZM_Projects Map! by DaOne");
        m_ClientMarkersCache = new ClientMarkersCache();
        m_Client3dMarkers = new array<ref VPP3DMarker>;
        m_Server3dMarkers = new array<ref VPP3DMarker>;
        m_ServerMarkers = new array<ref MarkerInfo>;
    }

    bool IsKeyboardBusy() {
        return m_IsKeyboardBusy;
    }

    void SetKeyboardBusy(bool isKeyboardBusy) {
        m_IsKeyboardBusy = isKeyboardBusy;
    }

    ref array<ref MarkerInfo> GetServerMarkers() {
        return m_ServerMarkers;
    }

    void SetServerMarkers(ref array<ref MarkerInfo> staticMarkers) {
        m_ServerMarkers = staticMarkers;
        foreach(ref MarkerInfo mkr: m_ServerMarkers)
        {
            mkr.Set3DForcedDisabled(!mkr.Is3DActive());
        }
        m_ServerMarkersSet = true;
        Display3dMarkers();
    }

    bool CanUse3DMarkers() {
        return m_CanUse3DMarkers;
    }

    void SetUse3DMarkers(bool canUse3DMarkers) {
        m_CanUse3DMarkers = canUse3DMarkers;
        m_Use3dMarkersSet = true;
        Display3dMarkers();
    }

    bool OwnPositionMarkerDisabled() {
        return m_OwnPositionMarkerDisabled;
    }

    void SetOwnPositionMarkerDisabled(bool ownPositionMarkerDisabled) {
        m_OwnPositionMarkerDisabled = ownPositionMarkerDisabled;
    }

    void SetForcePhysicalMap(bool state){
        m_ForcePhysicalMap = state;
    }

    bool ForcedPhysicalMap(){
        return m_ForcePhysicalMap;
    }

    void Display3dMarkers() {
        if (m_ServerMarkersSet && m_Use3dMarkersSet) {
            ref ClientMarkersCache cache = GetClientMarkersCache();
            if (cache) {
                SetClient3dMarkers(NULL); //Clear client 3d markers array
                SetServer3dMarkers(NULL); //Clear server 3d markers array
                cache.LoadCache(); //re-load the saved json incase raw edits were made.
                ref array<ref MarkerInfo> clientMarkers = cache.GetCachedArray();
                foreach(MarkerInfo clientMarker : clientMarkers) {
                    if (CanUse3DMarkers()) {
                        AddClient3dMarker( new VPP3DMarker(clientMarker.GetName(), clientMarker.GetIconPath(), clientMarker.GetPosition(), clientMarker.Is3DActive()) );
                    }
                }
                ref array<ref MarkerInfo> serverMarkers = GetServerMarkers();
                ref array<ref VPP3DMarker> marker3d = new array<ref VPP3DMarker>;
                if (serverMarkers != NULL) {
                    foreach(MarkerInfo serverMarker : serverMarkers) {
                        if(serverMarker.Is3DActive() && !serverMarker.Is3DForcedDisabled()) {
                            marker3d.Insert(new VPP3DMarker(serverMarker.GetName(),serverMarker.GetIconPath(),serverMarker.GetPosition(), true));
                        }
                    }
                    g_Game.SetServer3dMarkers(marker3d);
                }
            }
        }
    }

    /*
    	Return all active 3d markers.
    */
    ref array<ref VPP3DMarker> GetClient3dMarkers() {
        return m_Client3dMarkers;
    }

    void SetClient3dMarkers(ref array<ref VPP3DMarker> vpp3dMarkers) {
        if (!vpp3dMarkers) {
            // given array is null, just clear the existing one then
            foreach(ref VPP3DMarker marker3d : m_Client3dMarkers) {
                delete marker3d;
            }
            m_Client3dMarkers = new array<ref VPP3DMarker>;
        } else {
            m_Client3dMarkers = vpp3dMarkers;
        }
    }

    void AddClient3dMarker(ref VPP3DMarker marker3d) {
        if (marker3d && m_Client3dMarkers) {
            m_Client3dMarkers.Insert(marker3d);
        }
    }

    /*
    	Return global 3d markers
    */
    ref array<ref VPP3DMarker> GetServer3dMarkers() {
        return m_Server3dMarkers;
    }

    /*
    	For Global 3dMarkers
    */
    void SetServer3dMarkers(ref array<ref VPP3DMarker> markerArray) {
        if (markerArray != NULL) {
            m_Server3dMarkers = markerArray;
        } else {
            foreach(ref VPP3DMarker marker3d : m_Server3dMarkers) {
                delete marker3d;
            }
            m_Server3dMarkers = new array<ref VPP3DMarker>;
        }
    }

    void ShowAllHidden3dMarkers() {
        if (m_TempHidden3dMarkers) {
            foreach(ref VPP3DMarker marker3dToHide : m_TempHidden3dMarkers) {
                if (marker3dToHide) {
                    marker3dToHide.SetMarkerVisible(true);
                }
            }
            m_TempHidden3dMarkers = null;
        }
    }

    void HideAll3dMarkers() {
        if (!m_TempHidden3dMarkers) {
            m_TempHidden3dMarkers = new array<ref VPP3DMarker>;
            foreach(ref VPP3DMarker servermarker3d : m_Server3dMarkers) {
                if (servermarker3d.IsMarkerVisible()) {
                    m_TempHidden3dMarkers.Insert(servermarker3d);
                }
            }
            foreach(ref VPP3DMarker clientmarker3d : m_Client3dMarkers) {
                if (clientmarker3d.IsMarkerVisible()) {
                    m_TempHidden3dMarkers.Insert(clientmarker3d);
                }
            }
            foreach(ref VPP3DMarker marker3dToHide : m_TempHidden3dMarkers) {
                marker3dToHide.SetMarkerVisible(false);
            }
        }
    }

    void TempToggleAll3dMarkers() {
        if (m_TempHidden3dMarkers) {
            ShowAllHidden3dMarkers();
        } else {
            HideAll3dMarkers();
        }
    }

    /*
    	Returns Map Cache Manager instance ( it is auto inits @  DayZGame() )
    */
    ref ClientMarkersCache GetClientMarkersCache() {
        return m_ClientMarkersCache;
    }

    string GetConnectAddressPort() {
        return m_ConnectAddress + m_ConnectPort;
    }
}