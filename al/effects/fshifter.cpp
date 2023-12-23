
#include "config.h"

#include <optional>
#include <stdexcept>

#include "AL/al.h"
#include "AL/efx.h"

#include "alc/effects/base.h"
#include "effects.h"

#ifdef ALSOFT_EAX
#include <cassert>
#include "alnumeric.h"
#include "al/eax/exception.h"
#include "al/eax/utils.h"
#endif // ALSOFT_EAX


namespace {

std::optional<FShifterDirection> DirectionFromEmum(ALenum value)
{
    switch(value)
    {
    case AL_FREQUENCY_SHIFTER_DIRECTION_DOWN: return FShifterDirection::Down;
    case AL_FREQUENCY_SHIFTER_DIRECTION_UP: return FShifterDirection::Up;
    case AL_FREQUENCY_SHIFTER_DIRECTION_OFF: return FShifterDirection::Off;
    }
    return std::nullopt;
}
ALenum EnumFromDirection(FShifterDirection dir)
{
    switch(dir)
    {
    case FShifterDirection::Down: return AL_FREQUENCY_SHIFTER_DIRECTION_DOWN;
    case FShifterDirection::Up: return AL_FREQUENCY_SHIFTER_DIRECTION_UP;
    case FShifterDirection::Off: return AL_FREQUENCY_SHIFTER_DIRECTION_OFF;
    }
    throw std::runtime_error{"Invalid direction: "+std::to_string(static_cast<int>(dir))};
}

void Fshifter_setParamf(EffectProps *props, ALenum param, float val)
{
    switch(param)
    {
    case AL_FREQUENCY_SHIFTER_FREQUENCY:
        if(!(val >= AL_FREQUENCY_SHIFTER_MIN_FREQUENCY && val <= AL_FREQUENCY_SHIFTER_MAX_FREQUENCY))
            throw effect_exception{AL_INVALID_VALUE, "Frequency shifter frequency out of range"};
        props->Fshifter.Frequency = val;
        break;

    default:
        throw effect_exception{AL_INVALID_ENUM, "Invalid frequency shifter float property 0x%04x",
            param};
    }
}
void Fshifter_setParamfv(EffectProps *props, ALenum param, const float *vals)
{ Fshifter_setParamf(props, param, vals[0]); }

void Fshifter_setParami(EffectProps *props, ALenum param, int val)
{
    switch(param)
    {
    case AL_FREQUENCY_SHIFTER_LEFT_DIRECTION:
        if(auto diropt = DirectionFromEmum(val))
            props->Fshifter.LeftDirection = *diropt;
        else
            throw effect_exception{AL_INVALID_VALUE,
                "Unsupported frequency shifter left direction: 0x%04x", val};
        break;

    case AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION:
        if(auto diropt = DirectionFromEmum(val))
            props->Fshifter.RightDirection = *diropt;
        else
            throw effect_exception{AL_INVALID_VALUE,
                "Unsupported frequency shifter right direction: 0x%04x", val};
        break;

    default:
        throw effect_exception{AL_INVALID_ENUM,
            "Invalid frequency shifter integer property 0x%04x", param};
    }
}
void Fshifter_setParamiv(EffectProps *props, ALenum param, const int *vals)
{ Fshifter_setParami(props, param, vals[0]); }

void Fshifter_getParami(const EffectProps *props, ALenum param, int *val)
{
    switch(param)
    {
    case AL_FREQUENCY_SHIFTER_LEFT_DIRECTION:
        *val = EnumFromDirection(props->Fshifter.LeftDirection);
        break;
    case AL_FREQUENCY_SHIFTER_RIGHT_DIRECTION:
        *val = EnumFromDirection(props->Fshifter.RightDirection);
        break;
    default:
        throw effect_exception{AL_INVALID_ENUM,
            "Invalid frequency shifter integer property 0x%04x", param};
    }
}
void Fshifter_getParamiv(const EffectProps *props, ALenum param, int *vals)
{ Fshifter_getParami(props, param, vals); }

void Fshifter_getParamf(const EffectProps *props, ALenum param, float *val)
{
    switch(param)
    {
    case AL_FREQUENCY_SHIFTER_FREQUENCY:
        *val = props->Fshifter.Frequency;
        break;

    default:
        throw effect_exception{AL_INVALID_ENUM, "Invalid frequency shifter float property 0x%04x",
            param};
    }
}
void Fshifter_getParamfv(const EffectProps *props, ALenum param, float *vals)
{ Fshifter_getParamf(props, param, vals); }

EffectProps genDefaultProps() noexcept
{
    EffectProps props{};
    props.Fshifter.Frequency      = AL_FREQUENCY_SHIFTER_DEFAULT_FREQUENCY;
    props.Fshifter.LeftDirection  = *DirectionFromEmum(AL_FREQUENCY_SHIFTER_DEFAULT_LEFT_DIRECTION);
    props.Fshifter.RightDirection = *DirectionFromEmum(AL_FREQUENCY_SHIFTER_DEFAULT_RIGHT_DIRECTION);
    return props;
}

} // namespace

DEFINE_ALEFFECT_VTABLE(Fshifter);

const EffectProps FshifterEffectProps{genDefaultProps()};

#ifdef ALSOFT_EAX
namespace {

using FrequencyShifterCommitter = EaxCommitter<EaxFrequencyShifterCommitter>;

struct FrequencyValidator {
    void operator()(float flFrequency) const
    {
        eax_validate_range<FrequencyShifterCommitter::Exception>(
            "Frequency",
            flFrequency,
            EAXFREQUENCYSHIFTER_MINFREQUENCY,
            EAXFREQUENCYSHIFTER_MAXFREQUENCY);
    }
}; // FrequencyValidator

struct LeftDirectionValidator {
    void operator()(unsigned long ulLeftDirection) const
    {
        eax_validate_range<FrequencyShifterCommitter::Exception>(
            "Left Direction",
            ulLeftDirection,
            EAXFREQUENCYSHIFTER_MINLEFTDIRECTION,
            EAXFREQUENCYSHIFTER_MAXLEFTDIRECTION);
    }
}; // LeftDirectionValidator

struct RightDirectionValidator {
    void operator()(unsigned long ulRightDirection) const
    {
        eax_validate_range<FrequencyShifterCommitter::Exception>(
            "Right Direction",
            ulRightDirection,
            EAXFREQUENCYSHIFTER_MINRIGHTDIRECTION,
            EAXFREQUENCYSHIFTER_MAXRIGHTDIRECTION);
    }
}; // RightDirectionValidator

struct AllValidator {
    void operator()(const EAXFREQUENCYSHIFTERPROPERTIES& all) const
    {
        FrequencyValidator{}(all.flFrequency);
        LeftDirectionValidator{}(all.ulLeftDirection);
        RightDirectionValidator{}(all.ulRightDirection);
    }
}; // AllValidator

} // namespace

template<>
struct FrequencyShifterCommitter::Exception : public EaxException {
    explicit Exception(const char *message) : EaxException{"EAX_FREQUENCY_SHIFTER_EFFECT", message}
    { }
};

template<>
[[noreturn]] void FrequencyShifterCommitter::fail(const char *message)
{
    throw Exception{message};
}

bool EaxFrequencyShifterCommitter::commit(const EaxEffectProps &props)
{
    if(props == mEaxProps)
        return false;

    mEaxProps = props;

    auto get_direction = [](unsigned long dir) noexcept
    {
        if(dir == EAX_FREQUENCYSHIFTER_DOWN)
            return FShifterDirection::Down;
        if(dir == EAX_FREQUENCYSHIFTER_UP)
            return FShifterDirection::Up;
        return FShifterDirection::Off;
    };

    auto &eaxprops = std::get<EAXFREQUENCYSHIFTERPROPERTIES>(props);
    mAlProps.Fshifter.Frequency = eaxprops.flFrequency;
    mAlProps.Fshifter.LeftDirection = get_direction(eaxprops.ulLeftDirection);
    mAlProps.Fshifter.RightDirection = get_direction(eaxprops.ulRightDirection);

    return true;
}

void EaxFrequencyShifterCommitter::SetDefaults(EaxEffectProps &props)
{
    static constexpr EAXFREQUENCYSHIFTERPROPERTIES defprops{[]
    {
        EAXFREQUENCYSHIFTERPROPERTIES ret{};
        ret.flFrequency = EAXFREQUENCYSHIFTER_DEFAULTFREQUENCY;
        ret.ulLeftDirection = EAXFREQUENCYSHIFTER_DEFAULTLEFTDIRECTION;
        ret.ulRightDirection = EAXFREQUENCYSHIFTER_DEFAULTRIGHTDIRECTION;
        return ret;
    }()};
    props = defprops;
}

void EaxFrequencyShifterCommitter::Get(const EaxCall &call, const EaxEffectProps &props_)
{
    auto &props = std::get<EAXFREQUENCYSHIFTERPROPERTIES>(props_);
    switch(call.get_property_id())
    {
    case EAXFREQUENCYSHIFTER_NONE: break;
    case EAXFREQUENCYSHIFTER_ALLPARAMETERS: call.set_value<Exception>(props); break;
    case EAXFREQUENCYSHIFTER_FREQUENCY: call.set_value<Exception>(props.flFrequency); break;
    case EAXFREQUENCYSHIFTER_LEFTDIRECTION: call.set_value<Exception>(props.ulLeftDirection); break;
    case EAXFREQUENCYSHIFTER_RIGHTDIRECTION: call.set_value<Exception>(props.ulRightDirection); break;
    default: fail_unknown_property_id();
    }
}

void EaxFrequencyShifterCommitter::Set(const EaxCall &call, EaxEffectProps &props_)
{
    auto &props = std::get<EAXFREQUENCYSHIFTERPROPERTIES>(props_);
    switch(call.get_property_id())
    {
    case EAXFREQUENCYSHIFTER_NONE: break;
    case EAXFREQUENCYSHIFTER_ALLPARAMETERS: defer<AllValidator>(call, props); break;
    case EAXFREQUENCYSHIFTER_FREQUENCY: defer<FrequencyValidator>(call, props.flFrequency); break;
    case EAXFREQUENCYSHIFTER_LEFTDIRECTION: defer<LeftDirectionValidator>(call, props.ulLeftDirection); break;
    case EAXFREQUENCYSHIFTER_RIGHTDIRECTION: defer<RightDirectionValidator>(call, props.ulRightDirection); break;
    default: fail_unknown_property_id();
    }
}

#endif // ALSOFT_EAX
