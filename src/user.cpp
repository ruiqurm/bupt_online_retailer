#include "user.h"

const std::regex UserManager::USERNAME_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");
const std::regex UserManager::PASSWORD_PATTERN = std::regex("^[\\w,-_,!@#$%^&*()]{4,16}$");
