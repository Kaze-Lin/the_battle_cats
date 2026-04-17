set(SRC_FILES
    AppStart.cpp
    AppUpdate.cpp
    AppEnd.cpp

    Component/BackgroundImage.cpp
    Component/Button.cpp
    Component/Text.cpp
    Component/TextButton.cpp

    Phase/Phase.cpp
    Phase/Login.cpp
    Phase/CatBase.cpp
    Phase/Upgrade.cpp
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
)

set(INCLUDE_FILES
    App.hpp

    Component/BackgroundImage.hpp
    Component/Button.hpp
    Component/Text.hpp
    Component/TextButton.hpp

    ImageScaling.hpp
    SystemInfo.hpp

    Phase/Phase.hpp
    Phase/Login.hpp
    Phase/CatBase.hpp
    Phase/StageSelection.hpp
    Phase/Upgrade.hpp
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
)

set(TEST_FILES
)
