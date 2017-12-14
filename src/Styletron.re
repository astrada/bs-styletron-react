module Core = {
  type t;
  type options = {. prefix: string};
};

module Client = {
  [@bs.module "styletron-client"] [@bs.new]
  external createInstance :
    (Js.nullable(array(Dom.element)), Js.nullable(Core.options)) => Core.t =
    "default";
  let make = (~serverStyles=?, ~options=?, ()) =>
    createInstance(
      Js.Nullable.from_opt(serverStyles),
      Js.Nullable.from_opt(options)
    );
};

module React = {
  type statelessComponent =
    ReasonReact.component(
      ReasonReact.stateless,
      ReasonReact.noRetainedProps,
      ReasonReact.actionless
    );
  type styleObject('style) = Css.styleObject('style);
  type propsObject('props) = Js.t({..} as 'props);
  type rule('props, 'style) = Js.t({..} as 'props) => Js.t({..} as 'style);
  type base = [ | `String(string) | `ReactClass(ReasonReact.reactClass)];
  [@bs.module "styletron-react"] [@bs.val]
  external styled :
    (
      [@bs.unwrap] [ | `String(string) | `ReactClass(ReasonReact.reactClass)],
      rule('props, 'style)
    ) =>
    ReasonReact.reactClass =
    "";
  let makeStyledClass = (~base, ~rule) => styled(base, rule);
  let makeStyled = (~base, ~rule, ~props, children) => {
    let reactClass = styled(base, rule);
    ReasonReact.wrapJsForReason(~reactClass, ~props, children);
  };
  let makeStyledComponent = (~rule, ~component, ~make, children) => {
    let reactClass =
      ReasonReact.wrapReasonForJs(~component, jsProps =>
        make(~className=jsProps##className, children)
      );
    let styledClass = styled(`ReactClass(reactClass), rule);
    ReasonReact.wrapJsForReason(
      ~reactClass=styledClass,
      ~props=Js.Obj.empty(),
      children
    );
  };
  module Provider = {
    [@bs.module "styletron-react"]
    external reactClass : ReasonReact.reactClass = "StyletronProvider";
    let make = (~styletron=Client.make(), children) =>
      ReasonReact.wrapJsForReason(
        ~reactClass,
        ~props={"styletron": styletron},
        children
      );
  };
};
