/// This file has been generated, if you wish to
/// modify it in a permanent way, please refer
/// to the script file : gen/generator_python.rb

#include "interface.hh"

static PyObject* c_module;
static PyObject* py_module;
static PyObject* champ_module;

static void _init_python();

template <typename Lang, typename Cxx>
Lang cxx2lang(Cxx in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
PyObject* cxx2lang<PyObject*, int>(int in)
{
  return PyLong_FromLong(in);
}


template <>
PyObject* cxx2lang<PyObject*, double>(double in)
{
  return PyFloat_FromDouble(in);
}


template <>
PyObject* cxx2lang<PyObject*, std::string>(std::string in)
{
  return PyUnicode_FromString(in.c_str());
}


template <>
PyObject* cxx2lang<PyObject*, bool>(bool in)
{
  return PyBool_FromLong(in);
}

template <typename Cxx>
PyObject* cxx2lang_array(const std::vector<Cxx>& in)
{
  size_t size = in.size();
  PyObject* out = PyList_New(size);

  for (unsigned int i = 0; i < size; ++i)
    PyList_SET_ITEM(out, i, (cxx2lang<PyObject*, Cxx>(in[i])));

  return out;
}

template <typename Lang, typename Cxx>
Cxx lang2cxx(Lang in)
{
  return in.__if_that_triggers_an_error_there_is_a_problem;
}

template <>
int lang2cxx<PyObject*, int>(PyObject* in)
{
  long out = PyLong_AsLong(in);
  if (PyErr_Occurred())
    throw 42;
  return out;
}

template <>
double lang2cxx<PyObject*, double>(PyObject* in)
{
  double out = PyFloat_AsDouble(in);
  if (PyErr_Occurred())
    throw 42;
  return out;
}

template <>
bool lang2cxx<PyObject*, bool>(PyObject* in)
{
  return (bool)lang2cxx<PyObject*, int>(in);
}

template <>
std::string lang2cxx<PyObject*, std::string>(PyObject* in)
{
  const char * out = PyUnicode_AsUTF8(in);
  if (PyErr_Occurred())
    {
      throw 42;
    }
  return out;
}

template <typename Cxx>
std::vector<Cxx> lang2cxx_array(PyObject* in)
{
  if (!PyList_Check(in))
  {
    PyErr_SetString(PyExc_TypeError, "a list is required");
    throw 42;
  }

  std::vector<Cxx> out;
  unsigned int size = PyList_Size(in);

  for (unsigned int i = 0; i < size; ++i)
    out.push_back(lang2cxx<PyObject*, Cxx>(PyList_GET_ITEM(in, i)));

  return out;
}
/// Types de cases
template <>
PyObject* cxx2lang<PyObject*, case_type>(case_type in)
{
  PyObject* name = PyUnicode_FromString("case_type");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
case_type lang2cxx<PyObject*, case_type>(PyObject* in)
{
  return (case_type)lang2cxx<PyObject*, int>(in);
}

/// Direction
template <>
PyObject* cxx2lang<PyObject*, direction>(direction in)
{
  PyObject* name = PyUnicode_FromString("direction");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
direction lang2cxx<PyObject*, direction>(PyObject* in)
{
  return (direction)lang2cxx<PyObject*, int>(in);
}

/// Erreurs possibles
template <>
PyObject* cxx2lang<PyObject*, erreur>(erreur in)
{
  PyObject* name = PyUnicode_FromString("erreur");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
erreur lang2cxx<PyObject*, erreur>(PyObject* in)
{
  return (erreur)lang2cxx<PyObject*, int>(in);
}

/// Types d'actions
template <>
PyObject* cxx2lang<PyObject*, action_type>(action_type in)
{
  PyObject* name = PyUnicode_FromString("action_type");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
action_type lang2cxx<PyObject*, action_type>(PyObject* in)
{
  return (action_type)lang2cxx<PyObject*, int>(in);
}

/// Types de drapeaux de débug
template <>
PyObject* cxx2lang<PyObject*, debug_drapeau>(debug_drapeau in)
{
  PyObject* name = PyUnicode_FromString("debug_drapeau");
  PyObject* enm = PyObject_GetAttr(py_module, name);
  if (enm == NULL) throw 42;
  PyObject* arglist = Py_BuildValue("(i)", (int) in);
  PyObject* ret = PyObject_CallObject(enm, arglist);
  Py_DECREF(name);
  Py_DECREF(arglist);
  Py_DECREF(enm);
  return ret;
}

template <>
debug_drapeau lang2cxx<PyObject*, debug_drapeau>(PyObject* in)
{
  return (debug_drapeau)lang2cxx<PyObject*, int>(in);
}

/// Position dans la mine, donnée par deux coordonnées.
template <>
PyObject* cxx2lang<PyObject*, position>(position in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.ligne)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.colonne)));
  return tuple;
}

template <>
position lang2cxx<PyObject*, position>(PyObject* in)
{
  position out;
  PyObject* i;
  i = PyTuple_GetItem(in, 0);
  if (i == NULL) throw 42;
  out.ligne = lang2cxx<PyObject*, int>(i);
  i = PyTuple_GetItem(in, 1);
  if (i == NULL) throw 42;
  out.colonne = lang2cxx<PyObject*, int>(i);
  return out;
}

/// Minerai à récolter
template <>
PyObject* cxx2lang<PyObject*, minerai>(minerai in)
{
  PyObject* tuple = PyTuple_New(2);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, int>(in.resistance)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.rendement)));
  PyObject* name = PyUnicode_FromString("minerai");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
minerai lang2cxx<PyObject*, minerai>(PyObject* in)
{
  minerai out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.resistance = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.rendement = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

/// Nain (standard)
template <>
PyObject* cxx2lang<PyObject*, nain>(nain in)
{
  PyObject* tuple = PyTuple_New(6);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, position>(in.pos)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.vie)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, int>(in.pa)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, int>(in.pm)));
  PyTuple_SET_ITEM(tuple, 4, (cxx2lang<PyObject*, bool>(in.accroche)));
  PyTuple_SET_ITEM(tuple, 5, (cxx2lang<PyObject*, int>(in.butin)));
  PyObject* name = PyUnicode_FromString("nain");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
nain lang2cxx<PyObject*, nain>(PyObject* in)
{
  nain out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pos = lang2cxx<PyObject*, position>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.vie = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pa = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.pm = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(4);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.accroche = lang2cxx<PyObject*, bool>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(5);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.butin = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  return out;
}

/// Action de déplacement représentée dans l'historique.
template <>
PyObject* cxx2lang<PyObject*, action_hist>(action_hist in)
{
  PyObject* tuple = PyTuple_New(4);
  PyTuple_SET_ITEM(tuple, 0, (cxx2lang<PyObject*, action_type>(in.atype)));
  PyTuple_SET_ITEM(tuple, 1, (cxx2lang<PyObject*, int>(in.id_nain)));
  PyTuple_SET_ITEM(tuple, 2, (cxx2lang<PyObject*, direction>(in.dir)));
  PyTuple_SET_ITEM(tuple, 3, (cxx2lang<PyObject*, direction>(in.sens)));
  PyObject* name = PyUnicode_FromString("action_hist");
  PyObject* cstr = PyObject_GetAttr(py_module, name);
  Py_DECREF(name);
  if (cstr == NULL) throw 42;
  PyObject* ret = PyObject_CallObject(cstr, tuple);
  Py_DECREF(cstr);
  Py_DECREF(tuple);
  if (ret == NULL) throw 42;
  return ret;
}

template <>
action_hist lang2cxx<PyObject*, action_hist>(PyObject* in)
{
  action_hist out;
  PyObject* i;
  i = cxx2lang<PyObject*, int>(0);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.atype = lang2cxx<PyObject*, action_type>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(1);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.id_nain = lang2cxx<PyObject*, int>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(2);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.dir = lang2cxx<PyObject*, direction>(i);
  Py_DECREF(i);
  i = cxx2lang<PyObject*, int>(3);
  i = PyObject_GetItem(in, i);
  if (i == NULL) throw 42;
  out.sens = lang2cxx<PyObject*, direction>(i);
  Py_DECREF(i);
  return out;
}

/// Renvoie un chemin entre deux positions de la mine sous la forme d'une suite de directions à emprunter. Ce chemin minimise le nombre de blocs de granite nécessaire à casser. Si la position est invalide ou qu'il n'existe pas de tel chemin, le chemin renvoyé est vide.
static PyObject* p_chemin(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang_array(api_chemin(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, position>(a1)));
  } catch (...) { return NULL; }
}

/// Déplace le nain (standard) ``id_nain`` d'une case dans la direction choisie.
static PyObject* p_deplacer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_deplacer(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, direction>(a1)));
  } catch (...) { return NULL; }
}

/// Le nain (standard) ``id_nain`` lâche la paroi.
static PyObject* p_lacher(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_lacher(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Le nain (standard) ``id_nain`` s'agrippe à la paroi.
static PyObject* p_agripper(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_agripper(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Le nain (standard) ``id_nain`` mine le bloc ou le nain (standard) dans la direction indiquée.
static PyObject* p_miner(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_miner(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, direction>(a1)));
  } catch (...) { return NULL; }
}

/// Le nain (standard) ``id_nain`` tire la corde dans le sens donné (HAUT ou BAS).
static PyObject* p_tirer(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
PyObject* a2;
  if (!PyArg_ParseTuple(args, "OOO", &a0, &a1, &a2)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_tirer(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, direction>(a1), lang2cxx<PyObject*, direction>(a2)));
  } catch (...) { return NULL; }
}

/// Le nain (standard) ``id_nain`` pose une corde dans la direction indiquée.
static PyObject* p_poser_corde(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_poser_corde(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, direction>(a1)));
  } catch (...) { return NULL; }
}

/// Affiche le drapeau spécifié sur la case indiquée.
static PyObject* p_debug_afficher_drapeau(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, erreur>(api_debug_afficher_drapeau(lang2cxx<PyObject*, position>(a0), lang2cxx<PyObject*, debug_drapeau>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie le type d'une case donnée.
static PyObject* p_type_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, case_type>(api_type_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie la liste de toutes les positions occupées par une corde dans la mine.
static PyObject* p_liste_cordes(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_liste_cordes());
  } catch (...) { return NULL; }
}

/// Indique si une corde se trouve sur une case donnée.
static PyObject* p_corde_sur_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_corde_sur_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie le numéro du joueur à qui appartienent les nains (standard) sur la case indiquée. Renvoie -1 s'il n'y a pas de nain (standard) ou si la position est invalide.
static PyObject* p_nain_sur_case(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_nain_sur_case(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie la description du nain (standard) désigné par le numéro ``id_nain`` appartenant au joueur ``id_joueur``. Si le nain (standard)  n'est pas présent sur la carte, tous les membres de la structure ``nain`` renvoyée sont initialisés à -1 (et le champ ``accroche`` à `false`).
static PyObject* p_info_nain(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, nain>(api_info_nain(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, int>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie la liste de tous les minerais dans la mine.
static PyObject* p_liste_minerais(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_liste_minerais());
  } catch (...) { return NULL; }
}

/// Renvoie la description d'un minerai en fonction d'une position donnée. Si le minerai n'est pas présent sur la carte, ou si la position est invalide, tous les membres de la structure ``minerai`` renvoyée sont initialisés à -1.
static PyObject* p_info_minerai(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, minerai>(api_info_minerai(lang2cxx<PyObject*, position>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie le nombre de points de déplacement que coûterai le déplacement d'un nain (standard) dans une direction donnée. Renvoie -1 si le déplacement n'est pas possible.
static PyObject* p_cout_de_deplacement(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
PyObject* a1;
  if (!PyArg_ParseTuple(args, "OO", &a0, &a1)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_cout_de_deplacement(lang2cxx<PyObject*, int>(a0), lang2cxx<PyObject*, direction>(a1)));
  } catch (...) { return NULL; }
}

/// Renvoie la position de la taverne appartenant au joueur ``id_joueur``. Si le joueur n'existe pas, renvoie la position (-1, -1).
static PyObject* p_position_taverne(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, position>(api_position_taverne(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie la liste des actions effectuées par l’adversaire durant son tour, dans l'ordre chronologique. Les actions de débug n'apparaissent pas dans cette liste.
static PyObject* p_historique(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang_array(api_historique());
  } catch (...) { return NULL; }
}

/// Renvoie le score du joueur ``id_joueur``. Renvoie -1 si le joueur est invalide.
static PyObject* p_score(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_score(lang2cxx<PyObject*, int>(a0)));
  } catch (...) { return NULL; }
}

/// Renvoie votre numéro de joueur.
static PyObject* p_moi(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_moi());
  } catch (...) { return NULL; }
}

/// Renvoie le numéro de joueur de votre adversaire.
static PyObject* p_adversaire(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_adversaire());
  } catch (...) { return NULL; }
}

/// Annule la dernière action. Renvoie faux quand il n'y a pas d'action à annuler ce tour ci.
static PyObject* p_annuler(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, bool>(api_annuler());
  } catch (...) { return NULL; }
}

/// Retourne le numéro du tour actuel.
static PyObject* p_tour_actuel(PyObject* self, PyObject* args)
{
  (void)self;
  if (!PyArg_ParseTuple(args, "")) {
    return NULL;
  }
    try {
return cxx2lang<PyObject*, int>(api_tour_actuel());
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type case_type
static PyObject* p_afficher_case_type(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_case_type(lang2cxx<PyObject*, case_type>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type direction
static PyObject* p_afficher_direction(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_direction(lang2cxx<PyObject*, direction>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type erreur
static PyObject* p_afficher_erreur(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_erreur(lang2cxx<PyObject*, erreur>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type action_type
static PyObject* p_afficher_action_type(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_action_type(lang2cxx<PyObject*, action_type>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type debug_drapeau
static PyObject* p_afficher_debug_drapeau(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_debug_drapeau(lang2cxx<PyObject*, debug_drapeau>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type position
static PyObject* p_afficher_position(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_position(lang2cxx<PyObject*, position>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type minerai
static PyObject* p_afficher_minerai(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_minerai(lang2cxx<PyObject*, minerai>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type nain
static PyObject* p_afficher_nain(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_nain(lang2cxx<PyObject*, nain>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}

/// Affiche le contenu d'une valeur de type action_hist
static PyObject* p_afficher_action_hist(PyObject* self, PyObject* args)
{
  (void)self;
PyObject* a0;
  if (!PyArg_ParseTuple(args, "O", &a0)) {
    return NULL;
  }
    try {
api_afficher_action_hist(lang2cxx<PyObject*, action_hist>(a0));
  Py_INCREF(Py_None);
  return Py_None;
  } catch (...) { return NULL; }
}


/*
** Api functions to register.
*/
static PyMethodDef api_callback[] = {
  {"chemin", p_chemin, METH_VARARGS, "chemin"},  {"deplacer", p_deplacer, METH_VARARGS, "deplacer"},  {"lacher", p_lacher, METH_VARARGS, "lacher"},  {"agripper", p_agripper, METH_VARARGS, "agripper"},  {"miner", p_miner, METH_VARARGS, "miner"},  {"tirer", p_tirer, METH_VARARGS, "tirer"},  {"poser_corde", p_poser_corde, METH_VARARGS, "poser_corde"},  {"debug_afficher_drapeau", p_debug_afficher_drapeau, METH_VARARGS, "debug_afficher_drapeau"},  {"type_case", p_type_case, METH_VARARGS, "type_case"},  {"liste_cordes", p_liste_cordes, METH_VARARGS, "liste_cordes"},  {"corde_sur_case", p_corde_sur_case, METH_VARARGS, "corde_sur_case"},  {"nain_sur_case", p_nain_sur_case, METH_VARARGS, "nain_sur_case"},  {"info_nain", p_info_nain, METH_VARARGS, "info_nain"},  {"liste_minerais", p_liste_minerais, METH_VARARGS, "liste_minerais"},  {"info_minerai", p_info_minerai, METH_VARARGS, "info_minerai"},  {"cout_de_deplacement", p_cout_de_deplacement, METH_VARARGS, "cout_de_deplacement"},  {"position_taverne", p_position_taverne, METH_VARARGS, "position_taverne"},  {"historique", p_historique, METH_VARARGS, "historique"},  {"score", p_score, METH_VARARGS, "score"},  {"moi", p_moi, METH_VARARGS, "moi"},  {"adversaire", p_adversaire, METH_VARARGS, "adversaire"},  {"annuler", p_annuler, METH_VARARGS, "annuler"},  {"tour_actuel", p_tour_actuel, METH_VARARGS, "tour_actuel"},  {"afficher_case_type", p_afficher_case_type, METH_VARARGS, "afficher_case_type"},  {"afficher_direction", p_afficher_direction, METH_VARARGS, "afficher_direction"},  {"afficher_erreur", p_afficher_erreur, METH_VARARGS, "afficher_erreur"},  {"afficher_action_type", p_afficher_action_type, METH_VARARGS, "afficher_action_type"},  {"afficher_debug_drapeau", p_afficher_debug_drapeau, METH_VARARGS, "afficher_debug_drapeau"},  {"afficher_position", p_afficher_position, METH_VARARGS, "afficher_position"},  {"afficher_minerai", p_afficher_minerai, METH_VARARGS, "afficher_minerai"},  {"afficher_nain", p_afficher_nain, METH_VARARGS, "afficher_nain"},  {"afficher_action_hist", p_afficher_action_hist, METH_VARARGS, "afficher_action_hist"},  {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC PyInit__api()
{
  static struct PyModuleDef apimoduledef = {
      PyModuleDef_HEAD_INIT,
      "_api",
      "API module",
      -1,
      api_callback,
      NULL,
      NULL,
      NULL,
      NULL,
  };
  return PyModule_Create(&apimoduledef);
}


/*
** Load a Python module
*/

static PyObject* _import_module(const char* m)
{
  PyObject* name = PyUnicode_FromString(m);
  PyObject* module = PyImport_Import(name);
  Py_DECREF(name);
  if (module == NULL)
    if (PyErr_Occurred())
    {
      PyErr_Print();
      abort();
    }
  return module;
}

/*
** Inititialize python, register API functions,
** and load .py file
*/
static void _init_python()
{
  static wchar_t empty_string[] = L"";
  static wchar_t *argv[] = { (wchar_t *) &empty_string, NULL };

  const char* champion_path;

  champion_path = getenv("CHAMPION_PATH");
  if (champion_path == NULL)
    champion_path = ".";

  setenv("PYTHONPATH", champion_path, 1);

  static wchar_t program_name[] = L"stechec";
  Py_SetProgramName(program_name);

  PyImport_AppendInittab("_api", PyInit__api);
  Py_Initialize();
  PySys_SetArgvEx(1, argv, 0);

  champ_module = _import_module("prologin");
  py_module = _import_module("api");
}

/*
** Run a python function.
*/
static PyObject* _call_python_function(const char* name)
{
  static bool initialized = false;

  if (!initialized)
  {
    initialized = true;
    _init_python();
  }

  PyObject *arglist, *func;
  PyObject *result = NULL;

  func = PyObject_GetAttrString(champ_module, (char*)name);
  if (func && PyCallable_Check(func))
  {
    arglist = Py_BuildValue("()");
    result = PyEval_CallObject(func, arglist);
    Py_XDECREF(arglist);
    Py_DECREF(func);
  }
  if (result == NULL && PyErr_Occurred())
    PyErr_Print();

  return result;
}

/*
** Functions called from stechec to C.
*/
extern "C" void partie_init()
{
  PyObject* _retval = _call_python_function("partie_init");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void jouer_tour()
{
  PyObject* _retval = _call_python_function("jouer_tour");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

extern "C" void partie_fin()
{
  PyObject* _retval = _call_python_function("partie_fin");
  if (!_retval && PyErr_Occurred()) { PyErr_Print(); abort(); }
  try {
  Py_XDECREF(_retval);
  } catch (...) { PyErr_Print(); abort(); }
}

