//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <edit/Generated/Types.h>
#include <edit/ActionStack.h>

#include <obj/Vector.h>

namespace mud
{
	ActionStack::ActionStack()
		: m_done()
		, m_undone()
	{}

	void ActionStack::push(object_ptr<EditorAction> action)
	{
		m_undone.clear();
		//action->apply();
		m_done.push_back(std::move(action));
	}

	void ActionStack::redo()
	{
		if(m_undone.empty())
			return;

		m_undone.back()->apply();
		m_done.emplace_back(vector_pop(m_undone));
	}

	void ActionStack::undo()
	{
		if(m_done.empty())
			return;

		m_done.back()->undo();
		m_undone.emplace_back(vector_pop(m_done));
	}

	UndoTool::UndoTool(ToolContext& context)
		: Tool(context, "Undo", type<UndoTool>())
	{}

	ToolState UndoTool::start()
	{
		m_context.m_action_stack->undo();
		return ToolState::DONE;
	}

	RedoTool::RedoTool(ToolContext& context)
		: Tool(context, "Redo", type<RedoTool>())
	{}

	ToolState RedoTool::start()
	{
		m_context.m_action_stack->redo();
		return ToolState::DONE;
	}
}