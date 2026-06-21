set(SRC_FILES
    AppStart.cpp
    AppUpdate.cpp
    AppEnd.cpp

    Component/BackgroundImage.cpp
    Component/Button.cpp
    Component/Text.cpp
    Component/TwoLayerText.cpp
    Component/TextButton.cpp
    Component/Block/OptionBlock.cpp
    Component/Block/UpgradeBlock.cpp
    Component/Block/StageBlock.cpp
    Component/Block/DeployBlock.cpp
    Component/Wallet.cpp
    Component/Cannon.cpp
    Component/HealthBar.cpp


    Phase/Phase.cpp
    Phase/Login.cpp
    Phase/CatBase.cpp
    Phase/Upgrade.cpp
    Phase/TeamBuild.cpp
    Phase/StageSelection.cpp
    Phase/PropsStore.cpp
    Phase/Fight.cpp

    DataBase/UnitData.cpp
    DataBase/EnemyData.cpp
    DataBase/StageData.cpp

    PhaseManager.cpp
    DatabaseManager.cpp
    EntityManager.cpp
    LevelManager.cpp
    Unit.cpp
    AudioManager.cpp
)

set(INCLUDE_FILES
    App.hpp

    Component/BackgroundImage.hpp
    Component/Button.hpp
    Component/Text.hpp
    Component/TwoLayerText.hpp
    Component/TextButton.hpp
    Component/Block/OptionBlock.hpp
    Component/Block/UpgradeBlock.hpp
    Component/Block/StageBlock.hpp
    Component/Block/DeployBlock.hpp
    Component/Wallet.hpp
    Component/Cannon.hpp

    ImageScaling.hpp
    SystemInfo.hpp

    Phase/Phase.hpp
    Phase/Login.hpp
    Phase/CatBase.hpp
    Phase/StageSelection.hpp
    Phase/Upgrade.hpp
    Phase/TeamBuild.hpp
    Phase/PropsStore.hpp
    Phase/Fight.hpp

    DataBase/UnitData.hpp
    DataBase/EnemyData.hpp
    DataBase/StageData.hpp

    IStateful.hpp

    PhaseManager.hpp
    DatabaseManager.hpp
    EntityManager.hpp
    LevelManager.hpp
    StatCalculator.hpp
    Unit.hpp

    nlohmann/json.hpp
    UserData.hpp
    UserManager.hpp
    AudioManager.hpp

    Component/HealthBar.hpp
    Component/ScrollContainer.hpp
    Component/ResourceDisplay.hpp
)

set(TEST_FILES
)
