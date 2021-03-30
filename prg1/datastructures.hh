// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <unordered_map>
#include <memory>
#include <map>
#include <math.h>
#include <deque>
#include <algorithm>

// Types for IDs
using PlaceID = long long int;
using AreaID = long long int;
using Name = std::string;
using WayID = std::string;

// Return values for cases where required thing was not found
PlaceID const NO_PLACE = -1;
AreaID const NO_AREA = -1;
WayID const NO_WAY = "!!No way!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Enumeration for different place types
// !!Note since this is a C++11 "scoped enumeration", you'll have to refer to
// individual values as PlaceType::SHELTER etc.
enum class PlaceType { OTHER=0, FIREPIT, SHELTER, PARKING, PEAK, BAY, AREA, NO_TYPE };

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

struct Place {
    Place(PlaceID id, Name name, PlaceType type, Coord coordinate):
        id(id),
        name(name),
        type(type),
        coordinate(coordinate)
    {}
    PlaceID id;
    Name name;
    PlaceType type;
    Coord coordinate;
};

struct Area {
    Area(AreaID id, Name name, std::vector<Coord> coordinates):
        id(id),
        name(name),
        coordinates(coordinates),
        parent(nullptr),
        children({})
    {}
    AreaID id;
    Name name;
    std::vector<Coord> coordinates;
    std::shared_ptr<Area> parent;
    std::vector<std::weak_ptr<Area>> children;
};

double calculate_eucledean(Coord coord);

inline bool operator<(Coord c1, Coord c2)
{
    double c1_eucledean = calculate_eucledean(c1);
    double c2_eucledean = calculate_eucledean(c2);
    if ( c1_eucledean < c2_eucledean) { return true; }
    else if( c1_eucledean > c2_eucledean) {return false; }
    else if (c1.y < c2.y) { return true; }
    else { return false; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    int place_count();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Based on cppreference std::clear is linear in all cases
    void clear_all();

    // Estimate of performance: O(n)
    // Short rationale for estimate: Based on cppreference std::push_back is an amortized constant
    // and I loop through all items in id_datastructure causing n in the performance
    std::vector<PlaceID> all_places();

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate: Based on cppreference the average case of std::insert
    // is constant but the worst case is linear for an unordered_map
    bool add_place(PlaceID id, Name const& name, PlaceType type, Coord xy);

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate: Based on cppreference the average case of std::find is constant
    // but the worst case is linear std::end is constant
    std::pair<Name, PlaceType> get_place_name_type(PlaceID id);

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate: The average case of std::find is constant and worst case is linear
    Coord get_place_coord(PlaceID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: Based on cppreference std::insert for a multimap is always log(n)
    // and looping over the datastructure causes performance to be n log(n) and push_back is an
    // amortized constant
    std::vector<PlaceID> places_alphabetically();

    // Estimate of performance: O(n log(n))
    // Short rationale for estimate: Std::insert for a multimap is always log(n)
    // and looping over the datastructure causes performance to be n log(n) and push_back is an
    // amortized constant
    std::vector<PlaceID> places_coord_order();

    // Estimate of performance: O(n) average is amount of same named places
    // Short rationale for estimate: Based on cppreference std::equal_range is on average linear in number
    // of elements with the key, worst case is linear in the size of the container. Push_back is constant
    std::vector<PlaceID> find_places_name(Name const& name);

    // Estimate of performance: O(n) average is amount of same named places
    // Short rationale for estimate: Std::equal_range is on average linear in number
    // of elements with the key, worst case is linear in the size of the container. Push_back is constant
    std::vector<PlaceID> find_places_type(PlaceType type);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Std::find and equal_range are worst case linear. std::extract is based
    // on cppreference worstcase linear and average constant. But since its only done once
    // even though its in a for loop the performance is still O(n)
    bool change_place_name(PlaceID id, Name const& newname);

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate: std::find is on average a constant but worst case its linear
    bool change_place_coord(PlaceID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance: O(n) average is constant
    // Short rationale for estimate: std::insert for an unordered_map is on average constant but
    // worst case its linear
    bool add_area(AreaID id, Name const& name, std::vector<Coord> coords);

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate: The average case of std::find is constant
    // but the worst case is linear std::end is constant
    Name get_area_name(AreaID id);

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate: Based on cppreference the average case of std::find is constant
    // but the worst case is linear std::end is constant
    std::vector<Coord> get_area_coords(AreaID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::push_back for a vector is constant the loop causes the n
    std::vector<AreaID> all_areas();

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate:The average case of std::find is constant
    // but the worst case is linear
    bool add_subarea_to_area(AreaID id, AreaID parentid);

    // Estimate of performance: O(n) average is a constant
    // Short rationale for estimate:The average case of std::find is constant
    // but the worst case is linear also push_back for a vector is constant
    std::vector<AreaID> subarea_in_areas(AreaID id);

    // Non-compulsory operations

    // Estimate of performance: Unused O(1)
    // Short rationale for estimate: Unused
    void creation_finished();

    // Estimate of performance: O(n)
    // Short rationale for estimate: get_children is linear where in the worst case n is the container size
    //  and std::find is still worst case linear and average constant
    std::vector<AreaID> all_subareas_in_area(AreaID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: Equal_range is worst case linear to container size but on average
    // its linear to amount of members with the same key. All other operations are constant
    std::vector<PlaceID> places_closest_to(Coord xy, PlaceType type);

    // Estimate of performance: O(n) on average linear to amount of element with the key
    // Short rationale for estimate: Worst case is linear but since find and erase are on average constant
    // and equal_range causes the for loops to be the same size as equal_range which is on average
    // the amount of members with the same key
    bool remove_place(PlaceID id);

    // Estimate of performance: O(n)
    // Short rationale for estimate: std::mismatch is the only function not explained before and
    // based on cppreference its at most the size of the 2 containers combined
    AreaID common_area_of_subareas(AreaID id1, AreaID id2);

private:
    // Estimate of performance: O(n)
    // get_children is linear where in the worst case n is the container size
    std::vector<AreaID> get_children(std::shared_ptr<Area> Area);
    std::unordered_map<PlaceID, std::shared_ptr<Place>> id_datastructure_;
    std::unordered_multimap<Name, std::shared_ptr<Place>> name_datastructure_;
    std::unordered_multimap<PlaceType, std::shared_ptr<Place>> type_datastructure_;
    std::unordered_map<AreaID, std::shared_ptr<Area>> id_areastructure_;
    bool coord_changed_;
    bool name_changed_;
    std::vector<PlaceID> name_ordered_places_;
    std::vector<PlaceID> coord_ordered_places_;
};

#endif // DATASTRUCTURES_HH
