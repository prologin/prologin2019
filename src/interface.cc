/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2019 Prologin
*/

#include "api.hh"

#include <iostream>
#include <sstream>
#include <vector>

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
  os << "[";
  typename std::vector<T>::const_iterator it;
  for (it = arr.begin(); it != arr.end(); ++it)
  {
    if (it != arr.begin())
      os << ", ";
    os << *it;
  }
  os << "]";
  return os;
}


// todo avoir un ostringstream a la place de std::string

std::string convert_to_string(int i){
  std::ostringstream s;
  s << i;
  std::string result = s.str();
  return result;
}
std::string convert_to_string(double i){
  std::ostringstream s;
  s << i;
  std::string result = s.str();
  return result;
}
std::string convert_to_string(std::string i){
  return i;
}
std::string convert_to_string(bool i){
  return i?"true":"false";
}
std::string convert_to_string(std::vector<int> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(case_type in){
  switch (in)
  {
    case LIBRE: return "\"libre\"";
    case GRANITE: return "\"granite\"";
    case OBSIDIENNE: return "\"obsidienne\"";
    case MINERAI: return "\"minerai\"";
    case ERREUR: return "\"erreur\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<case_type> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(direction in){
  switch (in)
  {
    case HAUT: return "\"haut\"";
    case BAS: return "\"bas\"";
    case GAUCHE: return "\"gauche\"";
    case DROITE: return "\"droite\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<direction> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(erreur in){
  switch (in)
  {
    case OK: return "\"ok\"";
    case PA_INSUFFISANTS: return "\"pa_insuffisants\"";
    case PM_INSUFFISANTS: return "\"pm_insuffisants\"";
    case POSITION_INVALIDE: return "\"position_invalide\"";
    case HORS_LIMITES: return "\"hors_limites\"";
    case DIRECTION_INVALIDE: return "\"direction_invalide\"";
    case ID_NAIN_INVALIDE: return "\"id_nain_invalide\"";
    case DRAPEAU_INVALIDE: return "\"drapeau_invalide\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<erreur> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(debug_drapeau in){
  switch (in)
  {
    case AUCUN_DRAPEAU: return "\"aucun_drapeau\"";
    case DRAPEAU_BLEU: return "\"drapeau_bleu\"";
    case DRAPEAU_VERT: return "\"drapeau_vert\"";
    case DRAPEAU_ROUGE: return "\"drapeau_rouge\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<debug_drapeau> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(position in){
  std::string ligne = convert_to_string(in.ligne);
  std::string colonne = convert_to_string(in.colonne);
  std::string out = "{";
  out += "ligne:" + ligne;
  out += ", ";
  out += "colonne:" + colonne;
  return out + "}";
}

std::string convert_to_string(std::vector<position> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(minerai in){
  std::string resistance = convert_to_string(in.resistance);
  std::string rendement = convert_to_string(in.rendement);
  std::string out = "{";
  out += "resistance:" + resistance;
  out += ", ";
  out += "rendement:" + rendement;
  return out + "}";
}

std::string convert_to_string(std::vector<minerai> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(nain in){
  std::string pos = convert_to_string(in.pos);
  std::string vie = convert_to_string(in.vie);
  std::string pa = convert_to_string(in.pa);
  std::string pm = convert_to_string(in.pm);
  std::string butin = convert_to_string(in.butin);
  std::string out = "{";
  out += "pos:" + pos;
  out += ", ";
  out += "vie:" + vie;
  out += ", ";
  out += "pa:" + pa;
  out += ", ";
  out += "pm:" + pm;
  out += ", ";
  out += "butin:" + butin;
  return out + "}";
}

std::string convert_to_string(std::vector<nain> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
/// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
extern "C" erreur api_deplacer(int id_nain, direction dir)
{
  return api->deplacer(id_nain, dir);
}

/// Le nain (standard) ``id_nain`` s'accroche à la paroi et grimpe dans la direction choisie.
extern "C" erreur api_escalader(int id_nain, direction dir)
{
  return api->escalader(id_nain, dir);
}

/// Le nain (standard) ``id_nain`` s'accroche à la corde et se déplace le long de la corde.
extern "C" erreur api_grimper(int id_nain, direction dir)
{
  return api->grimper(id_nain, dir);
}

/// Le nain (standard) ``id_nain`` lâche la paroi ou la corde.
extern "C" erreur api_lacher(int id_nain)
{
  return api->lacher(id_nain);
}

/// Le nain (standard) ``id_nain`` creuse ou mine le bloc ou le nain (standard) dans la direction indiquée.
extern "C" erreur api_miner(int id_nain, direction dir)
{
  return api->miner(id_nain, dir);
}

/// Le nain (standard) ``id_nain`` tire dans un sens sur la corde.
extern "C" erreur api_tirer(int id_nain, direction dir_corde, direction sens)
{
  return api->tirer(id_nain, dir_corde, sens);
}

/// Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.
extern "C" erreur api_poser_corde(int id_nain, direction dir)
{
  return api->poser_corde(id_nain, dir);
}

/// Affiche le drapeau spécifié sur la case indiquée.
extern "C" erreur api_debug_afficher_drapeau(position pos, debug_drapeau drapeau)
{
  return api->debug_afficher_drapeau(pos, drapeau);
}

/// Renvoie le type d'une case donnée.
extern "C" case_type api_type_case(position pos)
{
  return api->type_case(pos);
}

/// Renvoie la liste de toutes les corde dans la mine.
extern "C" std::vector<position> api_liste_cordes()
{
  return api->liste_cordes();
}

/// Indique si une corde se trouve sur une case donnée.
extern "C" bool api_corde_sur_case(position pos)
{
  return api->corde_sur_case(pos);
}

/// Renvoie le numéro du joueur à qui appartient le nain (standard) sur la case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est invalide.
extern "C" int api_nain_sur_case(position pos)
{
  return api->nain_sur_case(pos);
}

/// Renvoie la description du nain (standard) désigné par le numéro ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas présent sur la carte, tous les membres de la structure ``nain`` renvoyée sont initialisés à -1.
extern "C" nain api_info_nain(int id_joueur, int id_nain)
{
  return api->info_nain(id_joueur, id_nain);
}

/// Renvoie la description d'un minerai en fonction d'une position donnée. Si le minerai n'est pas présent sur la carte, ou si la position est invalide, tous les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
extern "C" minerai api_info_minerrai(position pos)
{
  return api->info_minerrai(pos);
}

/// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est invalide.
extern "C" int api_score(int id_joueur)
{
  return api->score(id_joueur);
}

/// Renvoie votre numéro de joueur.
extern "C" int api_moi()
{
  return api->moi();
}

/// Renvoie le numéro de joueur de votre adversaire.
extern "C" int api_adversaire()
{
  return api->adversaire();
}

/// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à annuler ce tour-ci.
extern "C" bool api_annuler()
{
  return api->annuler();
}

/// Retourne le numéro du tour actuel.
extern "C" int api_tour_actuel()
{
  return api->tour_actuel();
}

/// Affiche le contenu d'une valeur de type case_type
std::ostream& operator<<(std::ostream& os, case_type v)
{
  switch (v) {
  case LIBRE: os << "LIBRE"; break;
  case GRANITE: os << "GRANITE"; break;
  case OBSIDIENNE: os << "OBSIDIENNE"; break;
  case MINERAI: os << "MINERAI"; break;
  case ERREUR: os << "ERREUR"; break;
  }
  return os;
}
extern "C" void api_afficher_case_type(case_type v)
{
  std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type direction
std::ostream& operator<<(std::ostream& os, direction v)
{
  switch (v) {
  case HAUT: os << "HAUT"; break;
  case BAS: os << "BAS"; break;
  case GAUCHE: os << "GAUCHE"; break;
  case DROITE: os << "DROITE"; break;
  }
  return os;
}
extern "C" void api_afficher_direction(direction v)
{
  std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type erreur
std::ostream& operator<<(std::ostream& os, erreur v)
{
  switch (v) {
  case OK: os << "OK"; break;
  case PA_INSUFFISANTS: os << "PA_INSUFFISANTS"; break;
  case PM_INSUFFISANTS: os << "PM_INSUFFISANTS"; break;
  case POSITION_INVALIDE: os << "POSITION_INVALIDE"; break;
  case HORS_LIMITES: os << "HORS_LIMITES"; break;
  case DIRECTION_INVALIDE: os << "DIRECTION_INVALIDE"; break;
  case ID_NAIN_INVALIDE: os << "ID_NAIN_INVALIDE"; break;
  case DRAPEAU_INVALIDE: os << "DRAPEAU_INVALIDE"; break;
  }
  return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
  std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type debug_drapeau
std::ostream& operator<<(std::ostream& os, debug_drapeau v)
{
  switch (v) {
  case AUCUN_DRAPEAU: os << "AUCUN_DRAPEAU"; break;
  case DRAPEAU_BLEU: os << "DRAPEAU_BLEU"; break;
  case DRAPEAU_VERT: os << "DRAPEAU_VERT"; break;
  case DRAPEAU_ROUGE: os << "DRAPEAU_ROUGE"; break;
  }
  return os;
}
extern "C" void api_afficher_debug_drapeau(debug_drapeau v)
{
  std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type position
std::ostream& operator<<(std::ostream& os, position v)
{
  os << "{ ";
  os << "ligne" << "=" << v.ligne;
  os << ", ";
  os << "colonne" << "=" << v.colonne;
  os << " }";
  return os;
}
extern "C" void api_afficher_position(position v)
{
  std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type minerai
std::ostream& operator<<(std::ostream& os, minerai v)
{
  os << "{ ";
  os << "resistance" << "=" << v.resistance;
  os << ", ";
  os << "rendement" << "=" << v.rendement;
  os << " }";
  return os;
}
extern "C" void api_afficher_minerai(minerai v)
{
  std::cerr << v << std::endl;
}

/// Affiche le contenu d'une valeur de type nain
std::ostream& operator<<(std::ostream& os, nain v)
{
  os << "{ ";
  os << "pos" << "=" << v.pos;
  os << ", ";
  os << "vie" << "=" << v.vie;
  os << ", ";
  os << "pa" << "=" << v.pa;
  os << ", ";
  os << "pm" << "=" << v.pm;
  os << ", ";
  os << "butin" << "=" << v.butin;
  os << " }";
  return os;
}
extern "C" void api_afficher_nain(nain v)
{
  std::cerr << v << std::endl;
}
