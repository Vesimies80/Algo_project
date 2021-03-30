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
    name_datastructure_({}),
    coord_changed_(true),
    name_changed_(true)
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    // Replace this comment with your implementation
}

int Datastructures::place_count()
{
    int size = static_cast<int>(id_datastructure_.size());
    return size;
}

void Datastructures::clear_all()
{
    id_datastructure_.clear();
    id_areastructure_.clear();
    name_datastructure_.clear();
    type_datastructure_.clear();
    coord_changed_ = true;
    name_changed_ = true;
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
    if ( not value)
    {
        return value;
    }
    name_datastructure_.insert({name, new_place});
    type_datastructure_.insert({type, new_place});
    coord_changed_ = true;
    name_changed_ = true;
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
    std::shared_ptr<Area> new_area = std::make_shared<Area>(id, name, coords);
    bool value = id_areastructure_.insert({id, new_area}).second;
    return value;
}

Name Datastructures::get_area_name(AreaID id)
{
    auto area = id_areastructure_.find(id);
    if ( area == id_areastructure_.end()){
        return NO_NAME;
    }
    Name result = area->second->name;
    return result;
}

std::vector<Coord> Datastructures::get_area_coords(AreaID id)
{
    auto area = id_areastructure_.find(id);
    if ( area == id_areastructure_.end()){
        return {NO_COORD};
    }
    auto result = area->second->coordinates;
    return result;
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // NOTE!! It's quite ok to leave this empty, if you don't need operations
    // that are performed after all additions have been done.
}


std::vector<PlaceID> Datastructures::places_alphabetically()
{
    if ( name_changed_)
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
    name_changed_ = false;
    return name_ordered_places_;
}

std::vector<PlaceID> Datastructures::places_coord_order()
{
    if ( coord_changed_)
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
    coord_changed_ = false;
    return coord_ordered_places_;
}

std::vector<PlaceID> Datastructures::find_places_name(Name const& name)
{
    std::vector<PlaceID> result;
    auto iterators = name_datastructure_.equal_range(name);
    for( auto iter = iterators.first; iter != iterators.second; ++iter )
    {
        result.push_back(iter->second->id);
    }
    return result;
}

std::vector<PlaceID> Datastructures::find_places_type(PlaceType type)
{
    std::vector<PlaceID> result;
    auto iterators = type_datastructure_.equal_range(type);
    for( auto iter = iterators.first; iter != iterators.second; ++iter )
    {
        result.push_back(iter->second->id);
    }
    return result;
}

bool Datastructures::change_place_name(PlaceID id, const Name& newname)
{
    auto place = id_datastructure_.find(id);
    if ( place == id_datastructure_.end())
    {
        return false;

    }
    auto iterator = name_datastructure_.equal_range(place->second->name);
    for ( auto iter = iterator.first; iter != iterator.second; ++iter)
    {
        if ( iter->second->id == id)
        {
            auto key_to_change = name_datastructure_.extract(iter);
            key_to_change.key() = newname;
            name_datastructure_.insert(std::move(key_to_change));
            break;
        }
    }
    place->second->name = newname;
    name_changed_ = true;
    return true;
}

bool Datastructures::change_place_coord(PlaceID id, Coord newcoord)
{
    auto place = id_datastructure_.find(id);
    if ( place == id_datastructure_.end())
    {
        return false;

    } else
    {
        place->second->coordinate = newcoord;
    }
    coord_changed_ = true;
    return true;
}

std::vector<AreaID> Datastructures::all_areas()
{
    std::vector<AreaID> result {};
    for( auto iter = id_areastructure_.begin(); iter != id_areastructure_.end(); ++iter )
    {
        result.push_back(iter->second->id);
    }
    return result;
}

bool Datastructures::add_subarea_to_area(AreaID id, AreaID parentid)
{
    auto area = id_areastructure_.find(id);
    auto parent_area = id_areastructure_.find(parentid);
    if ( (area == id_areastructure_.end()) || (parent_area == id_areastructure_.end()) || (area->second->parent != nullptr) )
    {
        return false;
    }
    area->second->parent = parent_area->second;
    parent_area->second->children.push_back(area->second);
    return true;
}

std::vector<AreaID> Datastructures::subarea_in_areas(AreaID id)
{
    auto iter = id_areastructure_.find(id);
    auto area = iter->second;
    if ( iter == id_areastructure_.end() )
    {
        return {NO_AREA};
    }
    std::vector<AreaID> result = {};

    while (area->parent != nullptr)
    {
        result.push_back(area->parent->id);
        area = area->parent;
    }
    return result;
}

std::vector<PlaceID> Datastructures::places_closest_to(Coord xy, PlaceType type)
{
    std::pair<double, std::shared_ptr<Place>> first = {NO_DISTANCE, nullptr};
    std::pair<double, std::shared_ptr<Place>> second = {NO_DISTANCE, nullptr};
    std::pair<double, std::shared_ptr<Place>> third = {NO_DISTANCE, nullptr};
    std::unordered_multimap<PlaceType, std::shared_ptr<Place>>::iterator start;
    std::unordered_multimap<PlaceType, std::shared_ptr<Place>>::iterator end;
    if ( type == PlaceType::NO_TYPE)
    {
        start = type_datastructure_.begin();
        end = type_datastructure_.end();
    } else {
        auto pair = type_datastructure_.equal_range(type);
        start = pair.first;
        end = pair.second;
    }
    for ( auto place = start; place != end; ++place)
    {
        double distance = calculate_eucledean({place->second.get()->coordinate.x-xy.x, place->second.get()->coordinate.y-xy.y});
        if (first.second == nullptr)
        {
            first = {distance, place->second};
        }
        else if ( (distance < first.first) || (distance == first.first && first.second.get()->coordinate.y > place->second.get()->coordinate.y))
        {
            third = second;
            second = first;
            first = {distance, place->second};
        }
        else if ( second.second == nullptr )
        {
            second = {distance, place->second};
        }
        else if (( distance < second.first) || (distance == second.first && second.second.get()->coordinate.y > place->second.get()->coordinate.y))
        {
            third = second;
            second = {distance, place->second};
        }
        else if ( third.second == nullptr)
        {
            third = {distance, place->second};
        }
        else if (( distance < third.first) || (distance == third.first && third.second.get()->coordinate.y > place->second.get()->coordinate.y))
        {
            third = {distance, place->second};
        }
    }
    std::vector<PlaceID> result {};
    if ( first.second != nullptr)
    {
        result.push_back(first.second.get()->id);
        if ( second.second != nullptr)
        {
            result.push_back(second.second.get()->id);
            if ( third.second != nullptr)
            {
                result.push_back(third.second.get()->id);
            }
        }
    }
    return result;
}

bool Datastructures::remove_place(PlaceID id)
{
    auto place = id_datastructure_.find(id);
    if ( place == id_datastructure_.end())
    {
        return false;
    }
    coord_changed_ = true;
    name_changed_ = true;
    auto iter = name_datastructure_.equal_range(place->second->name);
    for (auto it = iter.first; it != iter.second; ++it)
    {
        if (it->second.get()->id == id)
        {
            name_datastructure_.erase(it);
            break;
        }
    }
    auto iter2 = type_datastructure_.equal_range(place->second->type);
    for ( auto it2 = iter2.first; it2 != iter2.second; ++it2)
    {
        if (it2->second.get()->id == id)
        {
            type_datastructure_.erase(it2);
            break;
        }
    }
    id_datastructure_.erase(id);
    return true;
}

std::vector<AreaID> Datastructures::all_subareas_in_area(AreaID id)
{
    auto iter = id_areastructure_.find(id);
    if ( iter == id_areastructure_.end())
    {
        return {NO_AREA};

    }
    auto area = iter->second;
    std::vector<AreaID> result = get_children(area);
    return result;
}

AreaID Datastructures::common_area_of_subareas(AreaID id1, AreaID id2)
{
    auto area1 = id_areastructure_.find(id1);
    auto area2 = id_areastructure_.find(id2);
    if ( area1 == id_areastructure_.end() || area2 == id_areastructure_.end())
    {
        return NO_AREA;
    }
    std::deque<std::shared_ptr<Area>> parents1;
    auto parent1 = area1->second.get()->parent;
    while (parent1 != nullptr)
    {
        parents1.push_front(parent1);
        parent1 = parent1->parent;
    }
    std::deque<std::shared_ptr<Area>> parents2;
    auto parent2 = area2->second.get()->parent;
    while (parent2 != nullptr)
    {
        parents2.push_front(parent2);
        parent2 = parent2->parent;
    }
    auto result = std::mismatch(parents1.begin(),parents1.end(),parents2.begin(),parents2.end());
    if (result.first == parents1.begin())
    {
        return NO_AREA;
    }
    --result.first;
    return result.first->get()->id;
}

std::vector<AreaID> Datastructures::get_children(std::shared_ptr<Area> Area)
{
    std::vector<AreaID> result = {};
    for ( auto child = Area->children.begin(); child != Area->children.end(); ++child)
    {
        std::vector<AreaID> child_result = get_children(child->lock());
        for ( auto id:child_result )
        {
            result.push_back(id);
        }
        result.push_back(child->lock()->id);
    }
    return result;
}

double calculate_eucledean(Coord coord)
{
    return std::sqrt(std::pow(coord.x, 2) + std::pow(coord.y, 2));
}
