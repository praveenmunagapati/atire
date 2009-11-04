/*
 * ltw_run.cpp
 *
 *  Created on: Aug 3, 2009
 *      Author: monfee
 */

#include "ltw_run.h"
#include "task_a2b.h"
#include "task_f2f.h"
#include "corpus_txt.h"
#include "corpus.h"
#include "sys_file.h"

#include <string>

using namespace std;
using namespace QLINK;


ltw_run::ltw_run(char *configfile) : run(configfile)
{
	task_ = NULL;
	init();
}

ltw_run::~ltw_run()
{
	if (task_)
		delete task_;
}

void ltw_run::init()
{
	// set the corpus txt home
	if (get_config().get_value("TEARA_HOME").length() > 0) {
		corpus::instance().teara_home(get_home("TEARA_HOME"));
		corpus::instance().load_teara_map();
	}

	corpus_txt::instance().home(get_home("CORPUS_TXT"));
	if (get_config().get_value("TEARA_TXT").length() > 0) {
		corpus_txt::instance().teara_home(get_home("TEARA_TXT"));
		corpus_txt::instance().load_teara_map();
	}

	string taskname = get_config().get_value("task");
	string out_algor_name = get_config().get_algorithm_outgoing_name();
	string in_algor_name = get_config().get_algorithm_incoming_name();
	string::size_type pos = string::npos;
//	if ((pos = taskname.find("F2F")) != string::npos)
//		task_ = new task_f2f(taskname, out_algor_name, in_algor_name);
//	else if ((pos = taskname.find("A2B")) != string::npos)
//		task_ = new task_a2b(taskname, out_algor_name, in_algor_name);
//	else
//		throw std::runtime_error("No recognizable task given.");
	task_ = new ltw_task(taskname, out_algor_name, in_algor_name);

	task_->set_alorithm_bep(get_config().get_value("algorithm_bep"));
}

std::string ltw_run::get_home(const char *name)
{
	string home = get_config().get_value(name);
	if (home.length() == 0)
		home = ".";
	home.append(sys_file::SEPARATOR);
	return home;
}

void ltw_run::create()
{
	print_header();

	if (task_) {
		task_->perform();
	}

	print_footer();
}

void ltw_run::print()
{
	print_header();

	if (task_) {
		task_->print_links();
	}

	print_footer();
}

void ltw_run::print_header()
{
	printf(header.c_str(), run_id.c_str(), task.c_str());
}
