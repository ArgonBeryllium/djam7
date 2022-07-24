#include "states.h"
#include "boxer.h"

SwitchingState* SwitchingState::instance;
State::State(StateData* data_) : data(data_)
{
	get_next = [this]() { return new IdleState(data); };
}
IdleState::IdleState(StateData* data_) : State(data_)
{
	get_next = [this]() { return data->parent->is_player()? new IdleState(data) : data->parent->pickAction(); };
	cp = data->cp_idle;
	dur = data->dur_idle_base + cumt::common::frand()*data->dur_idle_range;
	vuln = data->parent->is_player()?BOTH:NONE;
}
