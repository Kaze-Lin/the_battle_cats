#include "App.hpp"
#include "UserManager.hpp"
#include "Util/Logger.hpp"

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
    UserManager::GetInstance().SaveDatabase();
}