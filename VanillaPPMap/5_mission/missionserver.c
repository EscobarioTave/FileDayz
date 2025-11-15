modded class MissionServer {
    private ref ServerMarkersCache m_ServerMarkersCache;
    void MissionServer() {
        m_ServerMarkersCache = new ServerMarkersCache();
        GetRPCManager().AddRPC( "RPC_MissionServer", "MarkerCheck", this, SingleplayerExecutionType.Server );
        GetRPCManager().AddRPC( "RPC_MissionServer", "OwnPositionMarkerDisabled", this, SingleplayerExecutionType.Server );
        GetRPCManager().AddRPC( "RPC_MissionServer", "SessionMarkers", this, SingleplayerExecutionType.Server );
        GetRPCManager().AddRPC( "RPC_MissionServer", "ForceMapItem", this, SingleplayerExecutionType.Server );
    }

    // This method is only relevant on the server. It will respond to each client with a bool saying whether or not 3d markers are allowed to be used.
    void MarkerCheck( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
        if (type == CallType.Server) GetRPCManager().SendRPC( "RPC_MissionGameplay", "MarkerCheck", new Param1<bool>(m_ServerMarkersCache.CanUse3DMarkers()), true, sender);
    }

    // This method is only relevant on the server. It will respond to each client with a bool saying whether or not 3d markers are allowed to be used.
    void OwnPositionMarkerDisabled( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
        if (type == CallType.Server) GetRPCManager().SendRPC( "RPC_MissionGameplay", "OwnPositionMarkerDisabled", new Param1<bool>(m_ServerMarkersCache.OwnPositionMarkerDisabled()), true, sender);
    }

    // This method is only relevant on the server. It will respond to each client with a list of all server markers.
    void SessionMarkers( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
        if (type == CallType.Server) {
            m_ServerMarkersCache.LoadCache();
            ref array<ref MarkerInfo> markerInfos = m_ServerMarkersCache.GetCachedArray();

            //Legacy code
            foreach (MarkerInfo markerInfo : markerInfos) {
                string iconPath = markerInfo.GetIconPath();
                iconPath.Replace("\\scripts\\", "\\");
                markerInfo.SetIconPath(iconPath);
            }
            GetRPCManager().SendRPC( "RPC_MissionGameplay", "SessionMarkers", new Param1<ref array<ref MarkerInfo>>(markerInfos), true, sender);
        }
    }

    void ForceMapItem(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target){
        if (type == CallType.Server) GetRPCManager().SendRPC( "RPC_MissionGameplay", "ForceMapItem", new Param1<bool>(m_ServerMarkersCache.ForceToUseMapItem()), true, sender);
    }
};