///
/// @file
/// @copyright Copyright (C) 2020
///
#include "rules/rules_factory.h"
#include "rules/conway_rules.h"

std::unique_ptr<GameRules> GameRulesFactory::MakeGameRules() {
  return std::unique_ptr<GameRules>(new ConwayRules());
}
