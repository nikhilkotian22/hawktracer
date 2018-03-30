#ifndef HAWKTRACER_PARSER_EVENT_HPP
#define HAWKTRACER_PARSER_EVENT_HPP

#include "parser/event_klass.hpp"

#include <hawktracer/base_types.h>

#include <unordered_map>

namespace HawkTracer
{
namespace parser
{

class Event
{
public:
    struct Value
    {
        Value(FieldType value, const EventKlassField* field) :
            value(std::move(value)), field(field)
        {
        }
        Value(Value&&) = default;
        Value(const Value& other);

        FieldType value;
        const EventKlassField* field;
    };

    explicit Event(const EventKlass& klass) :
        _klass(klass)
    {
    }
    Event(const Event& other) = default;
    Event(Event&&) = default;

    ~Event();

    void merge(Event event);

    template<typename T>
    T get_value(const std::string& key) const;

    template<typename T>
    T get_value_or_default(const std::string& key, const T& default_value) const
    {
        auto it = _values.find(key);
        return (it != _values.end()) ? get_value<T>(key) : default_value;
    }

    bool has_value(const std::string& key) const
    {
        return _values.find(key) != _values.end();
    }

    const Value& get_raw_value(const std::string& key) const
    {
        return _values.at(key);
    }

    const EventKlass& get_klass() const { return _klass; }

    const std::unordered_map<std::string, Value>& get_values() const { return _values; }

    HT_TimestampNs get_timestamp() const { return _timestamp; }

    void set_value(const EventKlassField* field, FieldType value);

private:
    std::unordered_map<std::string, Value> _values;
    const EventKlass& _klass;
    HT_TimestampNs _timestamp = (HT_TimestampNs)-1;
};

} // namespace parser
} // namespace HawkTracer

#endif // HAWKTRACER_PARSER_EVENT_HPP