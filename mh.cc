// Metaheuristics algorithm using GRASP approach with Simulated Annealing.
// Authors: Lluc Palou and Ramon Ventura.

#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// Definition of soccer player class.
struct Player {
  string name;
  string position;
  int price;
  string team;
  int points;
};

// Definition of player database data structure.
struct Player_database {
  vector<Player> porters;
  vector<Player> defenses;
  vector<Player> migcampistes;
  vector<Player> davanters;
};

// Definition and goalkeeper initialisation of query data structure.
struct Query {
  int def;
  int mig;
  int dav;
  int por = 1;
  int total_limit;
  int player_limit;
};

// Definition and initialisation of partial solution data structure.
struct Partial_solution {
  double time;
  int def_count = 0;
  int mig_count = 0;
  int dav_count = 0;
  int por_count = 0;
  int current_price = 0;
  int current_points = 0;
  int best_points = 0;
  vector<Player> players;
  vector<int> indexes;
};

// Definition of used players data structure.
struct Used_players {
  vector<bool> porters;
  vector<bool> defenses;
  vector<bool> migcampistes;
  vector<bool> davanters;
};

// Declaration of output file and timing
string output_file;
double start_time, end_time;
double now() { return clock() / double(CLOCKS_PER_SEC); }

// Boltzmann distribution temperature hyperparameter.
double temperature = 1e5;

/* Returns the most efficient player based on the following criteria, used to
sort the player database. */
bool compare_players_efficiency(const Player &a, const Player &b) {
  /* Ordering criteria: Efficciency points/price ratio with a penalization to
     expensive players. By manually experimenting with roots, the power
     function (0.35) has shown to be very effective. */

  // If both players have 0 points, order them based on price.
  if (a.points == 0 and b.points == 0) {
    return a.price < b.price;
  }

  // If one player has 0 points, it should be considered less efficient.
  if (a.points == 0) {
    return false;
  } else if (b.points == 0) {
    return true;

  } else {
    double efficiency_a = (a.points * 1.0) / pow(a.price, 0.35);
    double efficiency_b = (b.points * 1.0) / pow(b.price, 0.35);
    return efficiency_a > efficiency_b;
  }
}

// Applies ordering criteria to each of the vectors of positions.
void sort_players_by_points(Player_database &data_base) {
  sort(data_base.porters.begin(), data_base.porters.end(),
       compare_players_efficiency);
  sort(data_base.defenses.begin(), data_base.defenses.end(),
       compare_players_efficiency);
  sort(data_base.migcampistes.begin(), data_base.migcampistes.end(),
       compare_players_efficiency);
  sort(data_base.davanters.begin(), data_base.davanters.end(),
       compare_players_efficiency);
}

/* Reads and stores the soccer player database. Discards players that surpass
   the player limit price. */
Player_database read_data_base(string data_base, Query query_constraints) {
  Player_database database;

  ifstream in(data_base);
  while (not in.eof()) {
    Player player;

    // Reads raw soccer player data.
    getline(in, player.name, ';');
    if (player.name == "")
      break;
    getline(in, player.position, ';');
    in >> player.price;
    char aux;
    in >> aux;
    getline(in, player.team, ';');
    in >> player.points;
    string aux2;
    getline(in, aux2);

    // Price restriction
    if (player.price <= query_constraints.player_limit) {
      if (player.position == "por") {
        database.porters.push_back(player);
      } else if (player.position == "def") {
        database.defenses.push_back(player);
      } else if (player.position == "mig") {
        database.migcampistes.push_back(player);
      } else if (player.position == "dav") {
        database.davanters.push_back(player);
      }
    }
  }
  in.close();

  // Sorts database accordingly to the ordering criteria.
  sort_players_by_points(database);
  return database;
}

// Reads the given query. That is, player configurations and price condavaints.
Query read_query(string query) {
  Query query_constraints;

  ifstream in;
  in.open(query);
  in >> query_constraints.def >> query_constraints.mig >>
      query_constraints.dav >> query_constraints.total_limit >>
      query_constraints.player_limit;
  in.close();

  return query_constraints;
}

// Initialises the used players data structure.
Used_players initialise_used_players(const Player_database &database) {
  Used_players used;

  used.defenses = vector<bool>(database.defenses.size(), false);
  used.migcampistes = vector<bool>(database.migcampistes.size(), false);
  used.davanters = vector<bool>(database.davanters.size(), false);
  used.porters = vector<bool>(database.porters.size(), false);

  return used;
}

/* Auxiliar printing function. Given a position prints all the players from the
   solution that belong to it, in the correct format. */
void aux_write_solution(ofstream &out,
                        unordered_map<string, vector<string>> &players_position,
                        const string &position) {
  bool first = true;
  for (const string &name : players_position[position]) {
    if (!first) {
      out << ";" << name;
    } else {
      out << name;
      first = false;
    }
  }
  out << endl;
}

// Given a solution prints itself and its timing in the required format.
void write_solution(Partial_solution feasible_solution) {
  ofstream out(output_file);
  out.setf(ios::fixed);
  out.precision(1);

  out << feasible_solution.time << endl;

  // Creates a map to group soccer players by position and fills the map.
  unordered_map<string, vector<string>> players_position;
  for (const Player &player : feasible_solution.players) {
    players_position[player.position].push_back(player.name);
  }

  out << "POR: ";
  aux_write_solution(out, players_position, "por");
  out << "DEF: ";
  aux_write_solution(out, players_position, "def");
  out << "MIG: ";
  aux_write_solution(out, players_position, "mig");
  out << "DAV: ";
  aux_write_solution(out, players_position, "dav");

  out << "Punts: " << feasible_solution.best_points << endl;
  out << "Preu: " << feasible_solution.current_price << endl;

  out.close();
}

// Checks whether the query constraints are satisfied.
bool satisfies_query_constraints(const Query &query_constraints,
                                 const Partial_solution &feasible_solution) {
  return feasible_solution.def_count == query_constraints.def and
         feasible_solution.mig_count == query_constraints.mig and
         feasible_solution.dav_count == query_constraints.dav and
         feasible_solution.por_count == 1 and
         feasible_solution.current_price <= query_constraints.total_limit and
         feasible_solution.current_points > feasible_solution.best_points;
}

// References the appropriate player vector based on player position.
vector<Player> get_players(const Player_database &database, string position) {
  if (position == "def")
    return database.defenses;
  else if (position == "mig")
    return database.migcampistes;
  else if (position == "dav")
    return database.davanters;
  else if (position == "por")
    return database.porters;

  vector<Player> aux;
  return aux;
}

// References the appropriate player amount based on player position.
int &get_count(Partial_solution &feasible_solution, string position) {
  if (position == "def")
    return feasible_solution.def_count;
  else if (position == "mig")
    return feasible_solution.mig_count;
  else if (position == "dav")
    return feasible_solution.dav_count;
  else if (position == "por")
    return feasible_solution.por_count;

  static int aux;
  return aux;
}

// References the appropriate query constraint based on player position.
int get_query_constraint(const Query &query_constraints, string position) {
  if (position == "def")
    return query_constraints.def;
  else if (position == "mig")
    return query_constraints.mig;
  else if (position == "dav")
    return query_constraints.dav;
  else if (position == "por")
    return query_constraints.por;

  return 0;
}

/* References the appropriate boolean vector indicating used players based on
   player position. */
vector<bool> &get_used_players(Used_players &used, string position) {
  if (position == "def")
    return used.defenses;
  else if (position == "mig")
    return used.migcampistes;
  else if (position == "dav")
    return used.davanters;
  else if (position == "por")
    return used.porters;

  static vector<bool> aux;
  return aux;
}

/* Generates a feasible solution through a greedy algorithm, chosing first 11
   players that satisfy query constraints, ordered by defined criteria
   (efficiency ratio). */
void construct_greedy_solution(const Player_database &database,
                               const Query &query_constraints,
                               Used_players &used,
                               Partial_solution &feasible_solution,
                               const vector<string> &positions, int idx) {
  while (
      not satisfies_query_constraints(query_constraints, feasible_solution)) {
    string position = positions[idx];
    const vector<Player> &players = get_players(database, position);

    if (get_count(feasible_solution, position) <
        get_query_constraint(query_constraints, position)) {
      for (int i = 0; i < int(players.size()); ++i) {
        if (get_count(feasible_solution, position) <
            get_query_constraint(query_constraints, position)) {

          /* Pruning condition: checks whether adding the player exceeds the
             remaining budget. */
          if (feasible_solution.current_price + players[i].price <=
                  query_constraints.total_limit and
              not get_used_players(used, position)[i]) {

            // Updates soccer player position counter, price, and points.
            feasible_solution.players.push_back(players[i]);
            feasible_solution.indexes.push_back(i);
            get_used_players(used, position)[i] = true;
            get_count(feasible_solution, position)++;
            feasible_solution.current_price += players[i].price;
            feasible_solution.current_points += players[i].points;
          }
        }
      }
    }

    else if (idx + 1 < int(positions.size()))
      idx += 1;
  }
}

/* Allows to worsen a partial solution with probability given by the Boltzmann
   distribution. */
bool probability(int new_points, int old_points) {
  if (new_points == old_points or temperature == 0)
    return false;
  double n = rand() / RAND_MAX,
         p = exp(-(old_points - new_points) / temperature);
  if (n < p)
    return true;
  return false;
}

// Says whether a better solution has been found using simulated annealing.
bool improve_solution(const Player_database &database,
                      const Query &query_constraints, Used_players &used,
                      Partial_solution &feasible_solution) {
  bool found = false;

  // Generates a vector {0, 1, ..., n-1}.
  vector<int> random(11);
  iota(random.begin(), random.end(), 0);

  // Shuffles the elements randomly.
  random_shuffle(random.begin(), random.end());

  for (int i = 0; i < 11 and not found; ++i) {
    int idx = random[i];

    // Chooses one player from feasible solution at random to be changed.
    Player &player = feasible_solution.players[idx];
    string position = player.position;
    int price = feasible_solution.current_price - player.price;
    int points = feasible_solution.current_points - player.points;

    const vector<Player> &players = get_players(database, position);

    /* Will try to change only one player and see if solution improves with
       simulated annealing approach. */
    for (int j = 0; j < int(players.size()) and not found; ++j) {
      Player new_player = players[j];

      if (not get_used_players(used, position)[j] and
          (new_player.price + price <= query_constraints.total_limit) and
          ((new_player.points + points > feasible_solution.current_points) or
           probability(new_player.points, player.points))) {
        found = true;
        get_used_players(used, position)[feasible_solution.indexes[idx]] =
            false;
        get_used_players(used, position)[j] = true;
        feasible_solution.current_points = points + new_player.points;
        feasible_solution.current_price = price + new_player.price;
        feasible_solution.players[idx] = new_player;
        feasible_solution.indexes[idx] = j;

        if (feasible_solution.best_points < feasible_solution.current_points) {
          // Updates feasible solution atributes.
          end_time = now();
          feasible_solution.time = end_time - start_time;
          feasible_solution.best_points = feasible_solution.current_points;
          write_solution(feasible_solution);
        }
      }
    }

    // Updates temperature hyperparameter.
    temperature *= 0.999999;
  }

  return found;
}

// Main algorithm concerning metaheursitics with GRASP approach.
void grasp_mh(const Player_database &database, const Query &query_constraints,
              Used_players &used, Partial_solution &feasible_solution) {
  vector<string> positions = {"por", "def", "mig", "dav"};

  // Constructs greedy partial solution.
  construct_greedy_solution(database, query_constraints, used,
                            feasible_solution, positions, 0);
  feasible_solution.best_points = feasible_solution.current_points;

  // Applies simulated annealing.
  while (improve_solution(database, query_constraints, used, feasible_solution))
    ;
}

int main(int argc, char **argv) {
  string data_base;
  string query;

  data_base = argv[1];
  query = argv[2];
  output_file = argv[3];

  /* Firstly reads the query to store player limit. Allows us to filter them
     during the database reading process. */
  Query query_constraints = read_query(query);
  Player_database database = read_data_base(data_base, query_constraints);
  Used_players used = initialise_used_players(database);

  // Random generator seed.
  int rs = time(NULL);
  srand(rs);

  // Algorithm execution, solution writting, and timing.
  start_time = now();
  Partial_solution feasible_solution;
  grasp_mh(database, query_constraints, used, feasible_solution);
}
