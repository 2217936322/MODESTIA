#include "ModelChanger.hpp"
#include "../Helpers/Configs.hpp"
#include "../Helpers/Memory.hpp"

void Models::PlayerChanger(ClientFrameStage_t stage)
{
    if (stage != ClientFrameStage_t::FRAME_RENDER_START && stage != ClientFrameStage_t::FRAME_RENDER_END)
        return;

    static int originalIndex = 0;

    if (!g_LocalPlayer) 
    {
        originalIndex = 0;
        return;
    }

    constexpr auto getModel = [](int team) constexpr noexcept -> const char* 
    {
        constexpr std::array models{
        "models/player/custom_player/legacy/ctm_fbi_variantb.mdl",
        "models/player/custom_player/legacy/ctm_fbi_variantf.mdl",
        "models/player/custom_player/legacy/ctm_fbi_variantg.mdl",
        "models/player/custom_player/legacy/ctm_fbi_varianth.mdl",
        "models/player/custom_player/legacy/ctm_sas_variantf.mdl",
        "models/player/custom_player/legacy/ctm_st6_variante.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantg.mdl",
        "models/player/custom_player/legacy/ctm_st6_varianti.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantk.mdl",
        "models/player/custom_player/legacy/ctm_st6_variantm.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantf.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantg.mdl",
        "models/player/custom_player/legacy/tm_balkan_varianth.mdl",
        "models/player/custom_player/legacy/tm_balkan_varianti.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantj.mdl",
        "models/player/custom_player/legacy/tm_leet_variantf.mdl",
        "models/player/custom_player/legacy/tm_leet_variantg.mdl",
        "models/player/custom_player/legacy/tm_leet_varianth.mdl",
        "models/player/custom_player/legacy/tm_leet_varianti.mdl",
        "models/player/custom_player/legacy/tm_phoenix_variantf.mdl",
        "models/player/custom_player/legacy/tm_phoenix_variantg.mdl",
        "models/player/custom_player/legacy/tm_phoenix_varianth.mdl",

        "models/player/custom_player/legacy/tm_pirate.mdl",
        "models/player/custom_player/legacy/tm_pirate_varianta.mdl",
        "models/player/custom_player/legacy/tm_pirate_variantb.mdl",
        "models/player/custom_player/legacy/tm_pirate_variantc.mdl",
        "models/player/custom_player/legacy/tm_pirate_variantd.mdl",
        "models/player/custom_player/legacy/tm_anarchist.mdl",
        "models/player/custom_player/legacy/tm_anarchist_varianta.mdl",
        "models/player/custom_player/legacy/tm_anarchist_variantb.mdl",
        "models/player/custom_player/legacy/tm_anarchist_variantc.mdl",
        "models/player/custom_player/legacy/tm_anarchist_variantd.mdl",
        "models/player/custom_player/legacy/tm_balkan_varianta.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantb.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantc.mdl",
        "models/player/custom_player/legacy/tm_balkan_variantd.mdl",
        "models/player/custom_player/legacy/tm_balkan_variante.mdl",
        "models/player/custom_player/legacy/tm_jumpsuit_varianta.mdl",
        "models/player/custom_player/legacy/tm_jumpsuit_variantb.mdl",
        "models/player/custom_player/legacy/tm_jumpsuit_variantc.mdl"
        };

        switch (team) 
        {
        case 2: return static_cast<std::size_t>(g_Configs.misc.playerModelT - 1) < models.size() ? models[g_Configs.misc.playerModelT - 1] : nullptr;
        case 3: return static_cast<std::size_t>(g_Configs.misc.playerModelCT - 1) < models.size() ? models[g_Configs.misc.playerModelCT - 1] : nullptr;
        default: return nullptr;
        }
    };

    if (const auto model = getModel(g_LocalPlayer->m_iTeamNum())) 
    {
        if (stage == ClientFrameStage_t::FRAME_RENDER_START) 
        {
            originalIndex = g_LocalPlayer->m_nModelIndex();
            if (const auto modelprecache = g_NetworkStringTableContainer->FindTable("modelprecache")) 
            {
                modelprecache->AddString(false, model);
                const auto viewmodelArmConfig = g_Memory.GetPlayerViewmodelArmConfigForPlayerModel(model);
                modelprecache->AddString(false, viewmodelArmConfig[2]);
                modelprecache->AddString(false, viewmodelArmConfig[3]);
            }
        }

        const auto index = stage == ClientFrameStage_t::FRAME_RENDER_END && originalIndex ? originalIndex : g_MdlInfo->GetModelIndex(model);

        g_LocalPlayer->SetModelIndex(index);
    }
}

void Models::KnifeChanger(ClientFrameStage_t stage)
{
    if (stage != ClientFrameStage_t::FRAME_RENDER_START && stage != ClientFrameStage_t::FRAME_RENDER_END)
        return;

    if (!g_LocalPlayer)
        return;

    constexpr auto getModel = []() constexpr noexcept -> const char*
    {
        constexpr std::array models
        {
            "models/weapons/v_minecraft_pickaxe.mdl",
            "models/weapons/eminem/fidget_spinner/v_fidget_spinner.mdl"
        };

        return static_cast<std::size_t>(g_Configs.misc.knifeModel - 1) < models.size() ? models[g_Configs.misc.knifeModel - 1] : nullptr;
    };

    const auto viewmodelHandle = g_LocalPlayer->m_hViewModel();
    if (!viewmodelHandle.IsValid())
        return;

    const auto viewmodel = static_cast<C_BaseViewModel*>(g_EntityList->GetClientEntityFromHandle(viewmodelHandle));
    if (!viewmodel)
        return;

    static int originalIndex = 0;

    if (!viewmodel)
    {
        originalIndex = 0;
        return;
    }

    if (const auto model = getModel())
    {
        if (stage == ClientFrameStage_t::FRAME_RENDER_START)
        {
            originalIndex = viewmodel->m_nModelIndex();
            if (const auto modelprecache = g_NetworkStringTableContainer->FindTable("modelprecache"))
            {
                modelprecache->AddString(false, model);
            }
        }

        const auto index = stage == ClientFrameStage_t::FRAME_RENDER_END && originalIndex ? originalIndex : g_MdlInfo->GetModelIndex(model);

        if (viewmodel->m_hWeapon()->IsKnife())
        {
            viewmodel->SetModelIndex(index);
        }
    }
}

void Models::AWPChanger(ClientFrameStage_t stage)
{
    if (stage != ClientFrameStage_t::FRAME_RENDER_START && stage != ClientFrameStage_t::FRAME_RENDER_END)
        return;

    if (!g_LocalPlayer)
        return;

    constexpr auto getModel = []() constexpr noexcept -> const char*
    {
        constexpr std::array models
        {
            "models/weapons/eminem/dsr_50/v_dsr_50_v4.mdl"
        };

        return static_cast<std::size_t>(g_Configs.misc.awpModel - 1) < models.size() ? models[g_Configs.misc.awpModel - 1] : nullptr;
    };

    const auto viewmodelHandle = g_LocalPlayer->m_hViewModel();
    if (!viewmodelHandle.IsValid())
        return;

    const auto viewmodel = static_cast<C_BaseViewModel*>(g_EntityList->GetClientEntityFromHandle(viewmodelHandle));
    if (!viewmodel)
        return;

    static int originalIndex = 0;

    if (!viewmodel)
    {
        originalIndex = 0;
        return;
    }

    if (const auto model = getModel())
    {
        if (stage == ClientFrameStage_t::FRAME_RENDER_START)
        {
            originalIndex = viewmodel->m_nModelIndex();
            if (const auto modelprecache = g_NetworkStringTableContainer->FindTable("modelprecache"))
            {
                modelprecache->AddString(false, model);
            }
        }

        const auto index = stage == ClientFrameStage_t::FRAME_RENDER_END && originalIndex ? originalIndex : g_MdlInfo->GetModelIndex(model);

        if (viewmodel->m_hWeapon()->IsAwp())
        {
            viewmodel->SetModelIndex(index);
        }
    }
}