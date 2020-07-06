// Invoke with: scala -nobootcp -nc example.scala
// Otherwise you run into error: Compile server encountered fatal condition:
// javax/tools/DiagnosticListener which is an artifact of not using the "right"
// java version.

// value/mask tuples or low/high/mask triples to describe possible values.
trait PossibleValues
case class Tuple(var value: Int, var mask: Int) extends PossibleValues
case class Triple(var low: Int, var high: Int, var mask: Int) extends PossibleValues

// low-high range to describe possible values
case class CsRange(val low: Int, val high: Int)
implicit class Single(val value: Int) extends CsRange(value, value) {
    def --(that: Single) = CsRange(this.value, that.value)
}
case class Ranges(val ranges:List[CsRange])
object Ranges {
    def apply() = new Ranges(List())
    def apply(range: CsRange) = new Ranges(List(range))
    def apply(ranges: CsRange *) = new Ranges(ranges.toList)
}

trait GenericEntry
trait Context extends GenericEntry
trait Category extends GenericEntry
case class Hart(val ranges:Ranges, entries: Category*) extends Context

case class Description(val entries: GenericEntry*)

// Debug info pertaining to a hart.
trait DebugEntry
case class DebugTrigger(triggers: Ranges, matches:PossibleValues*) extends DebugEntry
case class DebugCategory(entries: DebugEntry*) extends Category

// Debug info pertaining to a Debug Module
trait DebugModuleEntry
case class DebugModuleAbstractCommands(matches:PossibleValues*) extends DebugModuleEntry
case class DebugModuleConnectedHarts(val ranges:Ranges) extends DebugModuleEntry
case class DebugModuleCategory(val ranges:Ranges, entries: DebugModuleEntry*) extends Category

// ISA-related info
trait IsaEntry
case class IsaXlen(val xlens:Int*) extends IsaEntry
case class IsaCategory(entries: IsaEntry*) extends Category

// Privilege mode stuff
trait PrivEntry
case class PrivModes(val modes:Char*) extends PrivEntry
case class PrivEpmp(val supported:Boolean) extends PrivEntry
case class PrivSatp(val modes:String*) extends PrivEntry
case class PrivCategory(entries: PrivEntry*) extends Category

// Fast interrupt stuff
trait FastIntEntry
case class FastIntMachineModeTimeRegAddr(val address:Number) extends FastIntEntry
case class FastIntMachineModeTimeCompareRegAddr(val address:Number) extends FastIntEntry
case class FastIntCategory(entries: FastIntEntry*) extends Category

// Trace stuff
trait TraceEntry
case class TraceBranchPredictorEntries(val entries:Number) extends TraceEntry
case class TraceJumpTargetCacheEntries(val entries:Number) extends TraceEntry
case class TraceContextBusWidth(val width:Number) extends TraceEntry
case class TraceCategory(entries: TraceEntry*) extends Category

val LOW = 0x1234
val HIGH = 0x5678
val MASK = 0xff00
val VALUE = LOW

var description = Description(
    Hart(Ranges(0),
        DebugCategory(
            DebugTrigger(Ranges(0 -- 3),
                Triple(LOW, HIGH, MASK)
            ),
            DebugTrigger(Ranges(4),
                Tuple(VALUE, MASK),
                Tuple(VALUE, MASK)
            )
        )
    ),
    Hart(Ranges(1 -- 4),
        DebugCategory(
            DebugTrigger(Ranges(0 -- 1),
                Triple(LOW, HIGH, MASK),
                Triple(LOW, HIGH, MASK)
            )
        )),
    Hart(Ranges(0, 2, 4),
        IsaCategory(
            IsaXlen(32, 64)
        ),
        PrivCategory(
            PrivModes('U', 'M', 'S'),
            PrivSatp("sv48", "sv64")
        )
    ),
    Hart(Ranges(1, 3),
        IsaCategory(
            IsaXlen(64)
        ),
        PrivCategory(
            PrivModes('M'),
            PrivEpmp(true)
        )
    ),
    DebugModuleCategory(Ranges(0),
        DebugModuleAbstractCommands(
            Triple(LOW, HIGH, MASK),
            Tuple(VALUE, MASK),
            Tuple(VALUE, MASK)
        ),
        DebugModuleConnectedHarts(Ranges(0 -- 4))
    ),
    // TODO: puzzle out flipping Hart() and FastIntCategory
    Hart(Ranges(1 -- 4),
        FastIntCategory(
            FastIntMachineModeTimeRegAddr(0x1234),
            FastIntMachineModeTimeCompareRegAddr(0x1234)
        )
    ),
    TraceCategory(
        TraceBranchPredictorEntries(0),
        TraceJumpTargetCacheEntries(0),
        TraceContextBusWidth(32)
    )
)

println(description)

/*
* category: Physical Memory
    * tuple: LOW, HIGH addresses
        * cacheable
        * LR/SC support
        * alignment and size restrictions
        * mode restrictions
    * tuple: LOW, HIGH addresses
        * ...
*/