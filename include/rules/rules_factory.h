///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef INCLUDE_RULES_FACTORY_H_
#define INCLUDE_RULES_FACTORY_H_
#include "rules.h"

#include <memory>

///
/// @brief The GameRulesFactory returns unique_ptr to default game rules
///
class GameRulesFactory {
public:
  static std::unique_ptr<GameRules> MakeGameRules();
};

#endif // INCLUDE_RULES_FACTORY_H_
