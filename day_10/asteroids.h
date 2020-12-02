#pragma once

#include <vector>

#include <Eigen/Core>

using Asteroid = Eigen::Vector2d;

bool is_between(const Eigen::Vector2d & start, const Eigen::Vector2d & end, const Eigen::Vector2d & point);
double angle(const Eigen::Vector2d & a, const Eigen::Vector2d & b);

std::vector<Asteroid> asteroids_from_file(const std::string & filename);
std::vector<Asteroid> convert_image(const std::vector<std::string> & line);
void remove(const Asteroid & a, std::vector<Asteroid> & asteroids);

bool can_see(const Asteroid & source, const Asteroid & target, const std::vector<Asteroid> & asteroids);
std::vector<Asteroid> all_can_see(const Asteroid & position, const std::vector<Asteroid> & asteroids);
int num_can_see(const Asteroid & source, const std::vector<Asteroid> & asteroids);
Asteroid best_asteroid(const std::vector<Asteroid> & asteroids);

std::vector<Asteroid> sorted_clockwise(const Asteroid & ref, std::vector<Asteroid> asteroids);
std::vector<Asteroid> vaporization_order(const Eigen::Vector2d & pos, std::vector<Asteroid> asteroids);
