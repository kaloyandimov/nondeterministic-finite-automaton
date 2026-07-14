#ifndef EXPORT_AUTOMATON_DOT_EXPORTER_HPP
#define EXPORT_AUTOMATON_DOT_EXPORTER_HPP

#include <filesystem>
#include <string>

class Automaton;

class AutomatonDotExporter {
 public:
   std::string to_string(const Automaton&) const;

   void export_to_file(const Automaton&, const std::filesystem::path&) const;

 private:
   static std::string escape(const std::string&);
};

#endif /* EXPORT_AUTOMATON_DOT_EXPORTER_HPP */
