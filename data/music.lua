
initialeventtable = {}
measures = 10
timesig = 2

for i=0, measures-1 do
    for j=0, timesig-1 do
        table.insert(initialeventtable,
            Event.new(MajorSet, StandardInterval, math.random(0,3), PopSet[i%4], 0, i*2+j))
            --Event.new(OctotonicSet, StandardInterval, , PopSet[i%4], 0, i*4+j))
    end
end

arp = xform(initialeventtable, identity, onbeat(timesig, 0))
chords = xform(initialeventtable, identity, onbeat(timesig, 0))
final = xform(initialeventtable, identity, onbeat(timesig, 0))

--initialeventtable= xform(initialeventtable, identity, inmeasure(8), true)

--final = xform(final, chordify(3), inmeasure(8))
--final = xform(final, identity, onbeat(0))
--final = xform(final, paramset("Length", 4))

arp = xform(arp, {
    sequence({ 0, 1, 2, 1, 0, 1, 2, 1}, 1/4),
    paramset("volume", .1),
    paramset("length", 1/4),
    paramset("wave", 1),
    octave(-1)
})
--arp = xform(arp, identity, inmeasure(8), true)

--arpclone = xform(arp, paramadd("Beat", 2))

--chords = xform(chords, chordify(3))
--chords = xform(chords, paramset("length", 4))
--chords = xform(chords, paramset("wavetype", 1))
--chords = xform(chords, pickup(-1, 1/2))

chords = xform(chords, {
    chordify(3),
    paramset("length", timesig),
    paramset("wavetype", 1),
    pickup(-1, 1/2) })

--chords = xform(chords, parammulti("volume", .5))
--chords = xform(chords, octave(-1))
--chords = xform(chords, octave(-1))

--chords = xform(chords, testxform)

merge = xform({
    --initialeventtable,
    chords,
    arp,
    --arpclone,
    --final
    })

return merge
