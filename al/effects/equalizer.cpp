
#include "config.h"

#include "AL/al.h"
#include "AL/efx.h"

#include "alc/effects/base.h"
#include "effects.h"

#ifdef ALSOFT_EAX
#include "alnumeric.h"
#include "al/eax/exception.h"
#include "al/eax/utils.h"
#endif // ALSOFT_EAX


namespace {

void Equalizer_setParami(EffectProps*, ALenum param, int)
{ throw effect_exception{AL_INVALID_ENUM, "Invalid equalizer integer property 0x%04x", param}; }
void Equalizer_setParamiv(EffectProps*, ALenum param, const int*)
{
    throw effect_exception{AL_INVALID_ENUM, "Invalid equalizer integer-vector property 0x%04x",
        param};
}
void Equalizer_setParamf(EffectProps *props, ALenum param, float val)
{
    switch(param)
    {
    case AL_EQUALIZER_LOW_GAIN:
        if(!(val >= AL_EQUALIZER_MIN_LOW_GAIN && val <= AL_EQUALIZER_MAX_LOW_GAIN))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer low-band gain out of range"};
        props->Equalizer.LowGain = val;
        break;

    case AL_EQUALIZER_LOW_CUTOFF:
        if(!(val >= AL_EQUALIZER_MIN_LOW_CUTOFF && val <= AL_EQUALIZER_MAX_LOW_CUTOFF))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer low-band cutoff out of range"};
        props->Equalizer.LowCutoff = val;
        break;

    case AL_EQUALIZER_MID1_GAIN:
        if(!(val >= AL_EQUALIZER_MIN_MID1_GAIN && val <= AL_EQUALIZER_MAX_MID1_GAIN))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer mid1-band gain out of range"};
        props->Equalizer.Mid1Gain = val;
        break;

    case AL_EQUALIZER_MID1_CENTER:
        if(!(val >= AL_EQUALIZER_MIN_MID1_CENTER && val <= AL_EQUALIZER_MAX_MID1_CENTER))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer mid1-band center out of range"};
        props->Equalizer.Mid1Center = val;
        break;

    case AL_EQUALIZER_MID1_WIDTH:
        if(!(val >= AL_EQUALIZER_MIN_MID1_WIDTH && val <= AL_EQUALIZER_MAX_MID1_WIDTH))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer mid1-band width out of range"};
        props->Equalizer.Mid1Width = val;
        break;

    case AL_EQUALIZER_MID2_GAIN:
        if(!(val >= AL_EQUALIZER_MIN_MID2_GAIN && val <= AL_EQUALIZER_MAX_MID2_GAIN))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer mid2-band gain out of range"};
        props->Equalizer.Mid2Gain = val;
        break;

    case AL_EQUALIZER_MID2_CENTER:
        if(!(val >= AL_EQUALIZER_MIN_MID2_CENTER && val <= AL_EQUALIZER_MAX_MID2_CENTER))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer mid2-band center out of range"};
        props->Equalizer.Mid2Center = val;
        break;

    case AL_EQUALIZER_MID2_WIDTH:
        if(!(val >= AL_EQUALIZER_MIN_MID2_WIDTH && val <= AL_EQUALIZER_MAX_MID2_WIDTH))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer mid2-band width out of range"};
        props->Equalizer.Mid2Width = val;
        break;

    case AL_EQUALIZER_HIGH_GAIN:
        if(!(val >= AL_EQUALIZER_MIN_HIGH_GAIN && val <= AL_EQUALIZER_MAX_HIGH_GAIN))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer high-band gain out of range"};
        props->Equalizer.HighGain = val;
        break;

    case AL_EQUALIZER_HIGH_CUTOFF:
        if(!(val >= AL_EQUALIZER_MIN_HIGH_CUTOFF && val <= AL_EQUALIZER_MAX_HIGH_CUTOFF))
            throw effect_exception{AL_INVALID_VALUE, "Equalizer high-band cutoff out of range"};
        props->Equalizer.HighCutoff = val;
        break;

    default:
        throw effect_exception{AL_INVALID_ENUM, "Invalid equalizer float property 0x%04x", param};
    }
}
void Equalizer_setParamfv(EffectProps *props, ALenum param, const float *vals)
{ Equalizer_setParamf(props, param, vals[0]); }

void Equalizer_getParami(const EffectProps*, ALenum param, int*)
{ throw effect_exception{AL_INVALID_ENUM, "Invalid equalizer integer property 0x%04x", param}; }
void Equalizer_getParamiv(const EffectProps*, ALenum param, int*)
{
    throw effect_exception{AL_INVALID_ENUM, "Invalid equalizer integer-vector property 0x%04x",
        param};
}
void Equalizer_getParamf(const EffectProps *props, ALenum param, float *val)
{
    switch(param)
    {
    case AL_EQUALIZER_LOW_GAIN:
        *val = props->Equalizer.LowGain;
        break;

    case AL_EQUALIZER_LOW_CUTOFF:
        *val = props->Equalizer.LowCutoff;
        break;

    case AL_EQUALIZER_MID1_GAIN:
        *val = props->Equalizer.Mid1Gain;
        break;

    case AL_EQUALIZER_MID1_CENTER:
        *val = props->Equalizer.Mid1Center;
        break;

    case AL_EQUALIZER_MID1_WIDTH:
        *val = props->Equalizer.Mid1Width;
        break;

    case AL_EQUALIZER_MID2_GAIN:
        *val = props->Equalizer.Mid2Gain;
        break;

    case AL_EQUALIZER_MID2_CENTER:
        *val = props->Equalizer.Mid2Center;
        break;

    case AL_EQUALIZER_MID2_WIDTH:
        *val = props->Equalizer.Mid2Width;
        break;

    case AL_EQUALIZER_HIGH_GAIN:
        *val = props->Equalizer.HighGain;
        break;

    case AL_EQUALIZER_HIGH_CUTOFF:
        *val = props->Equalizer.HighCutoff;
        break;

    default:
        throw effect_exception{AL_INVALID_ENUM, "Invalid equalizer float property 0x%04x", param};
    }
}
void Equalizer_getParamfv(const EffectProps *props, ALenum param, float *vals)
{ Equalizer_getParamf(props, param, vals); }

EffectProps genDefaultProps() noexcept
{
    EffectProps props{};
    props.Equalizer.LowCutoff = AL_EQUALIZER_DEFAULT_LOW_CUTOFF;
    props.Equalizer.LowGain = AL_EQUALIZER_DEFAULT_LOW_GAIN;
    props.Equalizer.Mid1Center = AL_EQUALIZER_DEFAULT_MID1_CENTER;
    props.Equalizer.Mid1Gain = AL_EQUALIZER_DEFAULT_MID1_GAIN;
    props.Equalizer.Mid1Width = AL_EQUALIZER_DEFAULT_MID1_WIDTH;
    props.Equalizer.Mid2Center = AL_EQUALIZER_DEFAULT_MID2_CENTER;
    props.Equalizer.Mid2Gain = AL_EQUALIZER_DEFAULT_MID2_GAIN;
    props.Equalizer.Mid2Width = AL_EQUALIZER_DEFAULT_MID2_WIDTH;
    props.Equalizer.HighCutoff = AL_EQUALIZER_DEFAULT_HIGH_CUTOFF;
    props.Equalizer.HighGain = AL_EQUALIZER_DEFAULT_HIGH_GAIN;
    return props;
}

} // namespace

DEFINE_ALEFFECT_VTABLE(Equalizer);

const EffectProps EqualizerEffectProps{genDefaultProps()};

#ifdef ALSOFT_EAX
namespace {

using EqualizerCommitter = EaxCommitter<EaxEqualizerCommitter>;

struct LowGainValidator {
    void operator()(long lLowGain) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Low Gain",
            lLowGain,
            EAXEQUALIZER_MINLOWGAIN,
            EAXEQUALIZER_MAXLOWGAIN);
    }
}; // LowGainValidator

struct LowCutOffValidator {
    void operator()(float flLowCutOff) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Low Cutoff",
            flLowCutOff,
            EAXEQUALIZER_MINLOWCUTOFF,
            EAXEQUALIZER_MAXLOWCUTOFF);
    }
}; // LowCutOffValidator

struct Mid1GainValidator {
    void operator()(long lMid1Gain) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Mid1 Gain",
            lMid1Gain,
            EAXEQUALIZER_MINMID1GAIN,
            EAXEQUALIZER_MAXMID1GAIN);
    }
}; // Mid1GainValidator

struct Mid1CenterValidator {
    void operator()(float flMid1Center) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Mid1 Center",
            flMid1Center,
            EAXEQUALIZER_MINMID1CENTER,
            EAXEQUALIZER_MAXMID1CENTER);
    }
}; // Mid1CenterValidator

struct Mid1WidthValidator {
    void operator()(float flMid1Width) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Mid1 Width",
            flMid1Width,
            EAXEQUALIZER_MINMID1WIDTH,
            EAXEQUALIZER_MAXMID1WIDTH);
    }
}; // Mid1WidthValidator

struct Mid2GainValidator {
    void operator()(long lMid2Gain) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Mid2 Gain",
            lMid2Gain,
            EAXEQUALIZER_MINMID2GAIN,
            EAXEQUALIZER_MAXMID2GAIN);
    }
}; // Mid2GainValidator

struct Mid2CenterValidator {
    void operator()(float flMid2Center) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Mid2 Center",
            flMid2Center,
            EAXEQUALIZER_MINMID2CENTER,
            EAXEQUALIZER_MAXMID2CENTER);
    }
}; // Mid2CenterValidator

struct Mid2WidthValidator {
    void operator()(float flMid2Width) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "Mid2 Width",
            flMid2Width,
            EAXEQUALIZER_MINMID2WIDTH,
            EAXEQUALIZER_MAXMID2WIDTH);
    }
}; // Mid2WidthValidator

struct HighGainValidator {
    void operator()(long lHighGain) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "High Gain",
            lHighGain,
            EAXEQUALIZER_MINHIGHGAIN,
            EAXEQUALIZER_MAXHIGHGAIN);
    }
}; // HighGainValidator

struct HighCutOffValidator {
    void operator()(float flHighCutOff) const
    {
        eax_validate_range<EqualizerCommitter::Exception>(
            "High Cutoff",
            flHighCutOff,
            EAXEQUALIZER_MINHIGHCUTOFF,
            EAXEQUALIZER_MAXHIGHCUTOFF);
    }
}; // HighCutOffValidator

struct AllValidator {
    void operator()(const EAXEQUALIZERPROPERTIES& all) const
    {
        LowGainValidator{}(all.lLowGain);
        LowCutOffValidator{}(all.flLowCutOff);
        Mid1GainValidator{}(all.lMid1Gain);
        Mid1CenterValidator{}(all.flMid1Center);
        Mid1WidthValidator{}(all.flMid1Width);
        Mid2GainValidator{}(all.lMid2Gain);
        Mid2CenterValidator{}(all.flMid2Center);
        Mid2WidthValidator{}(all.flMid2Width);
        HighGainValidator{}(all.lHighGain);
        HighCutOffValidator{}(all.flHighCutOff);
    }
}; // AllValidator

} // namespace

template<>
struct EqualizerCommitter::Exception : public EaxException {
    explicit Exception(const char* message) : EaxException{"EAX_EQUALIZER_EFFECT", message}
    { }
};

template<>
[[noreturn]] void EqualizerCommitter::fail(const char *message)
{
    throw Exception{message};
}

bool EaxEqualizerCommitter::commit(const EaxEffectProps &props)
{
    if(props == mEaxProps)
        return false;

    mEaxProps = props;

    auto &eaxprops = std::get<EAXEQUALIZERPROPERTIES>(props);
    mAlProps.Equalizer.LowGain = level_mb_to_gain(static_cast<float>(eaxprops.lLowGain));
    mAlProps.Equalizer.LowCutoff = eaxprops.flLowCutOff;
    mAlProps.Equalizer.Mid1Gain = level_mb_to_gain(static_cast<float>(eaxprops.lMid1Gain));
    mAlProps.Equalizer.Mid1Center = eaxprops.flMid1Center;
    mAlProps.Equalizer.Mid1Width = eaxprops.flMid1Width;
    mAlProps.Equalizer.Mid2Gain = level_mb_to_gain(static_cast<float>(eaxprops.lMid2Gain));
    mAlProps.Equalizer.Mid2Center = eaxprops.flMid2Center;
    mAlProps.Equalizer.Mid2Width = eaxprops.flMid2Width;
    mAlProps.Equalizer.HighGain = level_mb_to_gain(static_cast<float>(eaxprops.lHighGain));
    mAlProps.Equalizer.HighCutoff = eaxprops.flHighCutOff;

    return true;
}

void EaxEqualizerCommitter::SetDefaults(EaxEffectProps &props)
{
    static constexpr EAXEQUALIZERPROPERTIES defprops{[]
    {
        EAXEQUALIZERPROPERTIES ret{};
        ret.lLowGain = EAXEQUALIZER_DEFAULTLOWGAIN;
        ret.flLowCutOff = EAXEQUALIZER_DEFAULTLOWCUTOFF;
        ret.lMid1Gain = EAXEQUALIZER_DEFAULTMID1GAIN;
        ret.flMid1Center = EAXEQUALIZER_DEFAULTMID1CENTER;
        ret.flMid1Width = EAXEQUALIZER_DEFAULTMID1WIDTH;
        ret.lMid2Gain = EAXEQUALIZER_DEFAULTMID2GAIN;
        ret.flMid2Center = EAXEQUALIZER_DEFAULTMID2CENTER;
        ret.flMid2Width = EAXEQUALIZER_DEFAULTMID2WIDTH;
        ret.lHighGain = EAXEQUALIZER_DEFAULTHIGHGAIN;
        ret.flHighCutOff = EAXEQUALIZER_DEFAULTHIGHCUTOFF;
        return ret;
    }()};
    props = defprops;
}

void EaxEqualizerCommitter::Get(const EaxCall &call, const EaxEffectProps &props_)
{
    auto &props = std::get<EAXEQUALIZERPROPERTIES>(props_);
    switch(call.get_property_id())
    {
    case EAXEQUALIZER_NONE: break;
    case EAXEQUALIZER_ALLPARAMETERS: call.set_value<Exception>(props); break;
    case EAXEQUALIZER_LOWGAIN: call.set_value<Exception>(props.lLowGain); break;
    case EAXEQUALIZER_LOWCUTOFF: call.set_value<Exception>(props.flLowCutOff); break;
    case EAXEQUALIZER_MID1GAIN: call.set_value<Exception>(props.lMid1Gain); break;
    case EAXEQUALIZER_MID1CENTER: call.set_value<Exception>(props.flMid1Center); break;
    case EAXEQUALIZER_MID1WIDTH: call.set_value<Exception>(props.flMid1Width); break;
    case EAXEQUALIZER_MID2GAIN: call.set_value<Exception>(props.lMid2Gain); break;
    case EAXEQUALIZER_MID2CENTER: call.set_value<Exception>(props.flMid2Center); break;
    case EAXEQUALIZER_MID2WIDTH: call.set_value<Exception>(props.flMid2Width); break;
    case EAXEQUALIZER_HIGHGAIN: call.set_value<Exception>(props.lHighGain); break;
    case EAXEQUALIZER_HIGHCUTOFF: call.set_value<Exception>(props.flHighCutOff); break;
    default: fail_unknown_property_id();
    }
}

void EaxEqualizerCommitter::Set(const EaxCall &call, EaxEffectProps &props_)
{
    auto &props = std::get<EAXEQUALIZERPROPERTIES>(props_);
    switch(call.get_property_id())
    {
    case EAXEQUALIZER_NONE: break;
    case EAXEQUALIZER_ALLPARAMETERS: defer<AllValidator>(call, props); break;
    case EAXEQUALIZER_LOWGAIN: defer<LowGainValidator>(call, props.lLowGain); break;
    case EAXEQUALIZER_LOWCUTOFF: defer<LowCutOffValidator>(call, props.flLowCutOff); break;
    case EAXEQUALIZER_MID1GAIN: defer<Mid1GainValidator>(call, props.lMid1Gain); break;
    case EAXEQUALIZER_MID1CENTER: defer<Mid1CenterValidator>(call, props.flMid1Center); break;
    case EAXEQUALIZER_MID1WIDTH: defer<Mid1WidthValidator>(call, props.flMid1Width); break;
    case EAXEQUALIZER_MID2GAIN: defer<Mid2GainValidator>(call, props.lMid2Gain); break;
    case EAXEQUALIZER_MID2CENTER: defer<Mid2CenterValidator>(call, props.flMid2Center); break;
    case EAXEQUALIZER_MID2WIDTH: defer<Mid2WidthValidator>(call, props.flMid2Width); break;
    case EAXEQUALIZER_HIGHGAIN: defer<HighGainValidator>(call, props.lHighGain); break;
    case EAXEQUALIZER_HIGHCUTOFF: defer<HighCutOffValidator>(call, props.flHighCutOff); break;
    default: fail_unknown_property_id();
    }
}

#endif // ALSOFT_EAX
