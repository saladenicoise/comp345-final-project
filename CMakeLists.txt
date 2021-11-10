cmake_minimum_required(VERSION 3.20)
project(GameEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(comp345-final-project "Cards/Cards.h" "Cards/Cards.cpp" "Command/CommandProcessing.h" "Command/CommandProcessing.cpp" "Map/Map.h" "Map/Map.cpp" "Orders/Orders.h" "Orders/Orders.cpp" "Player/Player.h" "Player/Player.cpp" "GameEngine/GameEngine.h" "GameEngine/GameEngine.cpp" "GameEngine/GameEngineDriver.cpp")