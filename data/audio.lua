-- I needs some kind of table append function, Lua doesn't provide one.
function table.append(dest, src)
    for i, v in ipairs(src) do
        table.insert(dest, v)
    end
end

-------------------------------------------------------------------------------
-- Interval sets
-------------------------------------------------------------------------------
Set = {}
function Set.new(interval)
    local obj = {}
    local meta = {}
    local len = #interval
    local octave = 0
    for i, v in ipairs(interval) do
        octave = octave + v
    end
    function obj.length()
        return len
    end
    function obj.octave()
        return octave
    end
    function meta.__index(self, index)
        local value = 0
        local count = 0
        while index < 0 do
            index = index + len
            count = count - 1
        end
        while index >= len do
            index = index - len
            count = count + 1
        end

        --value = octave * math.floor(index / len)
        value = octave * count;

        local normalizedindex = math.fmod(index, len)
        for i = 1, normalizedindex do
            value = value + interval[i]
        end

        return value
    end
    setmetatable(obj, meta)
    return obj
end

-------------------------------------------------------------------------------
-- Setting up some basic sets based on scales and chords
-------------------------------------------------------------------------------
MajorSet = Set.new{ 2, 2, 1, 2, 2, 2, 1, }
OctotonicSet = Set.new{ 2, 1, 2, 1, 2, 1, 2, 1 }
MinorSet = Set.new{ 2, 1, 2, 2, 1, 2, 2, }
FifthsSet = Set.new{ 4, 4, 4, 4, 4, 4, 4, }

StandardInterval = Set.new{ 2, 2, 3 }

PopSet = Set.new{ 3, 1, 1, 1 }
LadderSet = Set.new{2, 3, -2, 1, -3}

-------------------------------------------------------------------------------
-- Library functions
-------------------------------------------------------------------------------

local EventMeta = {}
function EventMeta.__index(self, key)
    if key == "tone" then
        return self.scale[self.chord[self.value] + self.offset] + self.tonic
    else
        return nil
    end
end

Event = {}

function Event.clone(event)
    return Event.new(event.scale,
        event.chord, event.value, event.offset, event.tonic,
        event.beat, event.length, event.volume, event.wavetype)
end

-- An event with metadata attached so that it can be operated on by transormations
function Event.new(scale, chord, value, offset, tonic, beat, length, volume, wavetype)
    local event = {}
    setmetatable(event, EventMeta)
    event.clone = Event.clone

    event.scale = scale
    event.chord = chord
    event.value = value
    event.offset = offset

    event.tonic = tonic or 0
    event.beat = beat or 0
    event.length = length or 1
    event.volume = volume or .1
    event.wavetype = wavetype or 0

    return event
end

-------------------------------------------------------------------------------
-- Transformations
-------------------------------------------------------------------------------

-- Takes an input note and produces itself
function identity(event)
    return { event:clone() }
end

-- Creates a chord based on the event's scale, chord and offset
function sequence(chordvaluetable, Delay)
    Delay = Delay or 0
    return function(event)
        local newevents = {}
        for i, v in ipairs(chordvaluetable) do
            local newevent = event:clone()
            newevent.value = v
            newevent.beat = newevent.beat + (Delay * (i-1))
            table.insert(newevents, newevent)
        end
        return newevents
    end
end

-- Creates a chord based on the event's scale, chord and offset
function arpeggiate(Number, value, Delay)
    value = value or 0
    Delay = Delay or 0
    local chordvaluetable = {}
    for i=0, Number-1 do
        table.insert(chordvaluetable, value + i)
    end
    return sequence(chordvaluetable, Delay)
end

function echo(Number, Delay, Decay)
    return function(event)
        local newevents = {}
        for i=0, Number do
            local newevent = event:clone()
            newevent.beat = event.beat + (i * Delay)
            newevent.Volume = event.Volume * math.pow(Decay, i)
            table.insert(newevents, newevent)
        end
        return newevents
    end
end

function chordify(Number)
    return arpeggiate(Number)
end

-- Produces a function that adjusts one of the parameters of the original event
function paramset(param, newvalue)
    return function(event)
        newevent = event:clone()
        newevent[param] = newvalue;
        return { newevent }
    end
end

-- Produces a function that adjusts one of the parameters of the original event
function paramadd(param, newvalue)
    return function(event)
        newevent = event:clone()
        newevent[param] = newevent[param] + newvalue;
        return { newevent }
    end
end

-- Produces a function that adjusts one of the parameters of the original event
function parammulti(param, newvalue)
    return function(event)
        newevent = event:clone()
        newevent[param] = newevent[param] * newvalue;
        return { newevent }
    end
end

function pickup(diff, delay)
    return function(event)
        local newevent = event:clone()
        local pickupnote = event:clone()
        newevent.beat = newevent.beat + delay
        newevent.length = newevent.length - delay
        pickupnote.value = pickupnote.value + diff
        pickupnote.length = delay
        return { newevent, pickupnote }
    end
end

function translate(steps)
    return function(event)
        local newevent = event:clone()
        newevent.tonic = newevent.tonic + steps
        return { newevent }
    end
end

-- Translates over octaves
function octave(octaves)
    return function(event)
        local newevent = event:clone()
        newevent.tonic = newevent.tonic + (octaves * event.scale.octave())
        return { newevent }
    end
end

-------------------------------------------------------------------------------
-- Conditions
-------------------------------------------------------------------------------

function onbeat(beatsPerMeausure, beat)
    return function(event)
        return ((event.beat) % beatsPerMeausure) == beat
    end
end

function inmeasure(beatsPerMeasure, Measure)
    return function(event)
        return math.floor(event.beat/beatsPerMeasure) == Measure
    end
end

-------------------------------------------------------------------------------
-- Transformations and finalizations
-------------------------------------------------------------------------------

-- Evaluate a condition
function evalcond(cond, event, invert)
    local result = false
    if type(cond) == "boolean" then
        result = cond
    elseif type(cond) == "function" then
        result = cond(event)
    else
        result =  false
    end
    if invert then
        if result then result = false else result = true end
    end
    return result
end

-- Takes a single input event, and transforms it into a table  of new events.
-- Use mergeevents to take all events and put them into a single list in the end
function xform(eventtable, xfunc, cond, invert)
    xfunc = xfunc or identity
    cond = cond or true
    invert = invert or false
    local result = {}
    if type(xfunc) == "table" then
        result = eventtable
        for i, x in ipairs(xfunc) do
            result = xform(result, x, cond, invert)
        end
    else
        for i, event in ipairs(eventtable) do
            if type(event) == "table" and getmetatable(event) ~= EventMeta then
                table.append(result, xform(event, xfunc, cond, invert))
            elseif evalcond(cond, event, invert) then
                local neweventstable = xfunc(event)
                for k, v in pairs(neweventstable) do
                    table.insert(result, v)
                end
            end
        end
    end
    return result
end

function prepareaudio(audiofile)
    lastbeat = lastbeat or 0

    local mergedtable = require(audiofile)

    mergedtable = xform(mergedtable, paramadd("beat", 1));
    table.sort(mergedtable, function(e1, e2) return e1.beat < e2.beat end)

    --for i, v in ipairs(mergedtable) do
    --    print(v.beat, v.tone, v.volume, v.length)
    --end

    local lastevent = mergedtable[#mergedtable]

    if lastevent.beat + lastevent.length > lastbeat then
        lastbeat = lastevent.beat + lastevent.length
    end

    la.AddEvent(unpack(mergedtable))
end

