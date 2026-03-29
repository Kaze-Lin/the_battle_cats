set(SRC_FILES
    AppStart.cpp
    AppUpdate.cpp
    AppEnd.cpp

    Component/BackgroundImage.cpp
    Component/Button.cpp

    Phase/Phase.cpp
    Phase/CatBase.cpp
    Phase/Upgrade.cpp
    Phase/StageSelection.cpp
    Phase/PropsStore.cpp

    DataBase/UnitData.cpp
    DataBase/EnemyData.cpp

    PhaseManager.cpp
    DatabaseManager.cpp
)

set(INCLUDE_FILES
    App.hpp

    Component/BackgroundImage.hpp
    Component/Button.hpp

    ImageScaling.hpp
    SystemInfo.hpp

    Phase/Phase.hpp
    Phase/CatBase.hpp
    Phase/StageSelection.hpp
    Phase/Upgrade.hpp
    Phase/PropsStore.hpp

    DataBase/UnitData.hpp
    DataBase/EnemyData.hpp

    IStateful.hpp

    PhaseManager.hpp
    DatabaseManager.hpp

    nlohmann/json.hpp
)

set(TEST_FILES
)
