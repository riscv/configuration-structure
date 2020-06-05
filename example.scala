// Invoke with: scala -nobootcp -nc example.scala
// Otherwise you run into error: Compile server encountered fatal condition:
// javax/tools/DiagnosticListener which is an artifact of not using the "right"
// java version.

trait PossibleValues
case class Tuple(var value: Int, var mask: Int) extends PossibleValues
case class Triple(var low: Int, var high: Int, var mask: Int) extends PossibleValues

implicit class Single(val value: Int) extends Range(value, value+1, 1)
trait GenericEntry extends DebugEntry with IsaEntry
trait Context extends GenericEntry
trait Category extends GenericEntry
case class HartContext(val ranges:List[Range], entries: List[GenericEntry]) extends Context

// Debug-related info
trait DebugEntry
case class DebugTrigger(triggers: List[Range], matches:List[PossibleValues]) extends DebugEntry
case class DebugCategory(entries: List[DebugEntry]) extends Category

// ISA-related info
trait IsaEntry
class IsaXlen(val xlen:List[Int]) extends IsaEntry
case class IsaCategory(entries: List[IsaEntry]) extends Category


val LOW = 0x1234
val HIGH = 0x5678
val MASK = 0xff00

var description = List(
    HartContext(List(0), List(
        DebugCategory(List(
            DebugTrigger(List(Range(0, 3)), List(
                Triple(LOW, HIGH, MASK)
            ))
        ))
    )),
    HartContext(List(Range(1, 4)), List())
)

println("Description:")
println(description)

/*
* hart: 0
    * category: Debug
        * trigger: 0--3
            * triple: LOW, HIGH, MASK
        * trigger: 4
            * tuple: VALUE0, MASK0
            * tuple: VALUE1, MASK1
* hart: 1--4
    * category: Debug
        * trigger: 0--1
            * triple: LOW0, HIGH0, MASK0
            * triple: LOW1, HIGH1, MASK1
* hart: 0, 2, 4
    * category: ISA
        * xlen: 32, 64
    * category: Privileged
        * privilege modes: U, M, S
        * SATP supported modes: sv48, sv64
* hart: 1, 3
    * category: ISA
        * xlen: 64
    * category: Privileged
        * privilege modes: M
        * ePMP supported: true
* category: Debug
    * debug module: 0
        * abstract commands
            * triple: LOW, HIGH, MASK
            * tuple: VALUE0, MASK0
            * tuple: VALUE1, MASK1
        * connected harts: 0--4
* category: Fast interrupt
    * clic: 0
        * connected harts: 1--4
    * hart: 1--4
        * Machine Mode Time Register Address: 0x...
        * Machine Mode Time Compare Register Address: 0x...
* category: Trace
    * Number of entries in the branch predictor: 8
    * Number of entries in the jump target cache: 2
    * Width of context bus: 32
* category: Physical Memory
    * tuple: LOW, HIGH addresses
        * cacheable
        * LR/SC support
        * alignment and size restrictions
        * mode restrictions
    * tuple: LOW, HIGH addresses
        * ...

*/