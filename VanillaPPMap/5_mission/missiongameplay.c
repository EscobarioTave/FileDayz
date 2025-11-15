modded class MissionGameplay {
    private ref VPPMapMenu instance;

    void MissionGameplay() {
        GetRPCManager().AddRPC( "RPC_MissionGameplay", "MarkerCheck", this, SingleplayerExecutionType.Server );
        GetRPCManager().AddRPC( "RPC_MissionGameplay", "OwnPositionMarkerDisabled", this, SingleplayerExecutionType.Server );
        GetRPCManager().AddRPC( "RPC_MissionGameplay", "SessionMarkers", this, SingleplayerExecutionType.Server );
        GetRPCManager().AddRPC( "RPC_MissionGameplay", "ForceMapItem", this, SingleplayerExecutionType.Server );

        GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.GetMarkerData, 5000, false);
    }

    void GetMarkerData()
    {
        GetRPCManager().SendRPC( "RPC_MissionServer", "MarkerCheck", NULL, true);
        GetRPCManager().SendRPC( "RPC_MissionServer", "OwnPositionMarkerDisabled", NULL, true);
        GetRPCManager().SendRPC( "RPC_MissionServer", "SessionMarkers", NULL, true);
        GetRPCManager().SendRPC( "RPC_MissionServer", "ForceMapItem", NULL, true);
    }

    void OwnPositionMarkerDisabled( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
        if (type == CallType.Client) {
            Param1<bool> data;
            if ( !ctx.Read( data ) ) return;
            g_Game.SetOwnPositionMarkerDisabled(data.param1);
        }
    }

    void MarkerCheck( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
        if (type == CallType.Client) {
            Param1<bool> data;
            if ( !ctx.Read( data ) ) return;
            g_Game.SetUse3DMarkers(data.param1);
        }
    }

    void SessionMarkers( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
        if (type == CallType.Client) {
            Param1<ref array<ref MarkerInfo>> data;
            if ( !ctx.Read( data ) ) return;
            g_Game.SetServerMarkers(data.param1);
        }
    }

    void ForceMapItem(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target){
        if (type == CallType.Client) {
            Param1<bool> data;
            if ( !ctx.Read( data ) ) return;
            g_Game.SetForcePhysicalMap(data.param1);
        }
    }

    //Checks if server config forces player to have a physical map item to open the map, returns true if player has map, returns true if option is not forced, returns false if option is forced and player has not physical map in inventory
    bool CanOpenMap(){
        if (!g_Game.ForcedPhysicalMap())
            return true;

        if (g_Game.ForcedPhysicalMap()){
            //Check Inventory for map item
            array<EntityAI> itemsArray = new array<EntityAI>;
            PlayerBase pb;

            if (Class.CastTo(pb,GetGame().GetPlayer())){
                pb.GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, itemsArray); //INORDER,PREORDER
                
                for (int i = 0; i < itemsArray.Count(); i++){
                    if (itemsArray[i] != null){
                        if (itemsArray[i].IsInherited(ChernarusMap) || itemsArray[i].IsInherited(ItemMap) || itemsArray[i].IsKindOf("ChernarusMap"))
                            return true;
                    }
                }
            }
        }
        return false;
    }

    override void OnUpdate(float timeslice) {
        super.OnUpdate(timeslice);
        Input input = GetGame().GetInput();

        if (input.LocalPress("UAUIBack", false)) {
            autoptr VPPMapMenu mapMenu = VPPMapMenu.Cast( GetUIManager().FindMenu(VPP_MENU_MAP) );
            if (mapMenu != NULL && mapMenu.IsMenuOpen() && !g_Game.IsKeyboardBusy()) {
                instance.SetMenuOpen(false);
                GetGame().GetUIManager().HideScriptedMenu(instance);
                UnlockControls();
            }
        }

        if ( input.LocalPress("UAOpenVPPMap", false) ) {
            if (instance != null) {
                if (instance.IsMenuOpen() && !g_Game.IsKeyboardBusy()) {
                    //Hide Menu
                    instance.SetMenuOpen(false);
                    GetGame().GetUIManager().HideScriptedMenu(instance);
                    UnlockControls();
                } else if (GetGame().GetUIManager().GetMenu() == NULL && CanOpenMap()) {
                    //Show Menu
                    GetGame().GetUIManager().ShowScriptedMenu(instance, NULL);
                    instance.SetMenuOpen(true);
                    instance.DisplayClientMarkers();
                    instance.DisplayServerMarkers();
                    LockControls();
                }
            } else if (GetGame().GetUIManager().GetMenu() == NULL && instance == null && CanOpenMap()) {
                //Create Menu
                LockControls();
                instance = VPPMapMenu.Cast(GetUIManager().EnterScriptedMenu(VPP_MENU_MAP, null));
                instance.SetMenuOpen(true);
            }
        }

        if ( input.LocalPress("UAToggleVPPMap3DMarkers", false) ) {
            if (!g_Game.IsKeyboardBusy()) {
                g_Game.TempToggleAll3dMarkers();
            }
        }
    }

    private void LockControls() {
        GetGame().GetMission().PlayerControlDisable(INPUT_EXCLUDE_ALL);
        GetGame().GetUIManager().ShowUICursor( true );
        GetGame().GetMission().GetHud().Show( false );
    }

    private void UnlockControls() {
        GetGame().GetMission().PlayerControlEnable(false);
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor( false );
        GetGame().GetMission().GetHud().Show( true );
    }
}