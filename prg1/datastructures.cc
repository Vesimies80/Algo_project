// Datastructures.cc

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

Datastructures::Datastructures():
    id_datastructure_({}),
    data_changed_(true)
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    int size = id_datastructure_.size();
    return size;
}

void Datastructures::clear_all()
{
    id_datastructure_.clear();
    flag_updator(true);
}

std::vector<PlaceID> Datastructures::all_places()
{
    std::vector<PlaceID> all_place {};
    for( auto iter = id_datastructure_.begin(); iter != id_datastructure_.end(); ++iter )
    {
        all_place.push_back(iter->first);
    }
    return all_place;
}

bool Datastructures::add_place(PlaceID id, const Name& name, PlaceType type, Coord xy)
{
    std::shared_ptr<Place> new_place = std::make_shared<Place>(id, name, type, xy);
    bool value = id_datastructure_.insert({id, new_place}).second;
    flag_updator(true);
    return value;
}

std::pair<Name, PlaceType> Datastructures::get_place_name_type(PlaceID id)
{
    auto iterator = id_datastructure_.find(id);
    if( iterator == id_datastructure_.end() )
    {
        return {NO_NAME, PlaceType::NO_TYPE};
    } else {
        auto place = iterator->second;
        return {place->name, place->type};
    }
}

Coord Datastructures::get_place_coord(PlaceID id)
{
    auto iterator = id_datastructure_.find(id);
    if( iterator == id_datastructure_.end() ){
        return NO_COORD;
    } else {
        auto place = iterator->second;
        return place->coordinate;
    }
}

bool Datastructures::add_area(AreaID id, const Name &name, std::vector<Coord> coords)
{
    // Replace this comment with your implementation
    return false;
}

Name Datastructures::get_area_name(AreaID id)
{
    // Replace this comment with your implementation
    return NO_NAME;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_COORD};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    if ( data_changed_)
    {
        std::multimap<Name, std::shared_ptr<Place>> map_of_names;
        name_ordered_places_ = {};
        for( auto iter = id_datastructure_.begin(); iter != id_datastructure_.end(); ++iter )
        {
            auto place = iter->second;
            map_of_names.insert({place->name, place});
        }
        for (auto iter = map_of_names.begin(); iter != map_of_names.end(); ++iter)
        {
            name_ordered_places_.push_back(iter->second->id);
        }
    }
    flag_updator(false);
    return name_ordered_places_;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    if ( data_changed_)
    {
        std::multimap<Coord, std::shared_ptr<Place>> map_of_names;
        coord_ordered_places_ = {};
        for( auto iter = id_datastructure_.begin(); iter != id_datastructure_.end(); ++iter )
        {
            auto place = iter->second;
            map_of_names.insert({place->coordinate, place});
        }
        for (auto iter = map_of_names.begin(); iter != map_of_names.end(); ++iter)
        {
            coord_ordered_places_.push_back(iter->second->id);
        }
    }
    flag_updator(false);
    return coord_ordered_places_;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    // Replace this comment with your implementation
    return {};
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    // Replace this comment with your implementation
    return false;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_areas()
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    // Replace this comment with your implementation
    return {};
}

bool Datastructures::remove_place(PlaceID id)
{
    // Replace this comment with your implementation
    return false;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    // Replace this comment with your implementation
    return {NO_AREA};
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    // Replace this comment with your implementation
    return NO_AREA;
}

void Datastructures::flag_updator(bool data)
{
    data_changed_ = data;
}

double calculate_eucledean(Coord coord)
{
    return std::sqrt(std::pow(coord.x, 2) + std::pow(coord.y, 2));
}
