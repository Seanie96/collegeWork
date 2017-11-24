using System;
using System.Collections.Generic;
 
namespace Tastier { 

public class Obj { // properties of declared symbol
   public string name; // its name
   public int kind;    // var, proc or scope
   public int type;    // its type if var (undef for proc)
   public int level;   // lexic level: 0 = global; >= 1 local
   public int adr;     // address (displacement) in scope 
   public Obj next;    // ptr to next object in scope
   public bool isArray = false; // defaults as scalar
   public int dimensions = 0;
   public int length1 = 0, length2 = 0;
   // for scopes
   public Obj outer;   // ptr to enclosing scope
   public Obj locals;  // ptr to locally declared objects
   public int nextAdr; // next free address in scope
}

public class SymbolTable {

   const int // object kinds
      var = 0, proc = 1, scope = 2, constant = 3, array = 4; 

   const int // types
      undef = 0, integer = 1, boolean = 2;

   public Obj topScope; // topmost procedure scope
   public int curLevel; // nesting level of current scope
   public Obj undefObj; // object node for erroneous symbols

   public bool mainPresent;
   
   Parser parser;
   
   public SymbolTable(Parser parser) {
      curLevel = -1; 
      topScope = null;
      undefObj = new Obj();
      undefObj.name = "undef";
      undefObj.kind = var;
      undefObj.type = undef;
      undefObj.level = 0;
      undefObj.adr = 0;
      undefObj.next = null;
      this.parser = parser;
      mainPresent = false;
   }

// open new scope and make it the current scope (topScope)
   public void OpenScope() {
      Obj scop = new Obj();
      scop.name = "";
      scop.kind = scope; 
      scop.outer = topScope; 
      scop.locals = null;
      scop.nextAdr = 0;
      topScope = scop; 
      curLevel++;
   }

// close current scope
   public void CloseScope() {
      Obj items = topScope.locals;

      int type, kind, lexicalLevel;
      string typeName, kindName, lexicalLevelName;

      while ( items != null) {
        type = items.type;
        kind = items.kind;
        lexicalLevel = items.level;

        if(type == 0){
          typeName = "undef";
        }else if(type == 1){
          typeName = "integer";
        }else{
          typeName = "boolean";
        }

        if(kind == 0){
          kindName = "var";
        }else if(kind == 1){
          kindName = "proc";
        }else if(kind == 2){
          kindName = "scope";
        }else{
          kindName = "constant";
        }

        if(lexicalLevel == 0){
          lexicalLevelName = "global";
        }else{
          lexicalLevelName = "local";
        }

        Console.WriteLine(";Name: {0}, Type: {1}, Kind: {2}, Level: {3}", items.name, typeName, kindName, lexicalLevelName);

        items = items.next;
      }

    topScope = topScope.outer;
    curLevel--;
   }

// open new sub-scope and make it the current scope (topScope)
   public void OpenSubScope() {
   // lexic level remains unchanged
      Obj scop = new Obj();
      scop.name = "";
      scop.kind = scope;
      scop.outer = topScope;
      scop.locals = null;
   // next available address in stack frame remains unchanged
      scop.nextAdr = topScope.nextAdr;
      topScope = scop;
   }

// close current sub-scope
   public void CloseSubScope() {
   // update next available address in enclosing scope
      topScope.outer.nextAdr = topScope.nextAdr;
   // lexic level remains unchanged
      topScope = topScope.outer;
   }

   public int getLength1(Obj obj) {
    return obj.length1;
   }

// create new object node in current scope
   public Obj NewObj(string name, int kind, int type, int dims, int len1, int len2) {
      Obj p, last; 
      Obj obj = new Obj();
      obj.name = name; obj.kind = kind;
      obj.type = type; obj.level = curLevel; 
      obj.length1 = len1;
      obj.length2 = len2;
      obj.dimensions = dims;
      obj.next = null; 
      p = topScope.locals; last = null;
      while (p != null) { 
         if (p.name == name)
            parser.SemErr("name declared twice");
         last = p; p = p.next;
      }
      if (last == null)
         topScope.locals = obj; else last.next = obj;
      if (kind >= var) {
         obj.adr = topScope.nextAdr++;
        if (kind == array) {
          if (obj.dimensions == 2) {
            topScope.nextAdr += (obj.length1 * obj.length2) - 1;
          } else  {
            topScope.nextAdr += (obj.length1) - 1;
          }
        }
      }
      return obj;
   }

// search for name in open scopes and return its object node
   public Obj Find(string name) {
      Obj obj, scope;
      scope = topScope;
      while (scope != null) { // for all open scopes
         obj = scope.locals;
         while (obj != null) { // for all objects in this scope
            if (obj.name == name) return obj;
            obj = obj.next;
         }
         scope = scope.outer;
      }
      parser.SemErr(name + " is undeclared");
      return undefObj;
   }

} // end SymbolTable

} // end namespace
