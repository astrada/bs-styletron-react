module Core: {type t; type options = {. prefix: string};};

module Client: {
  let make: (~serverStyles: array(Dom.element)=?, ~options: Core.options=?, unit) => Core.t;
};

module React: {
  type statelessComponent =
    ReasonReact.component(
      ReasonReact.stateless,
      ReasonReact.noRetainedProps,
      ReasonReact.actionless
    );
  type styleObject('style) = Js.t(({..} as 'style));
  type propsObject('props) = Js.t(({..} as 'props));
  type rule('props, 'style) = propsObject('props) => styleObject('style);
  type base = [ | `String(string) | `ReactClass(ReasonReact.reactClass)];
  let makeStyledClass: (~base: base, ~rule: rule('props, 'style)) => ReasonReact.reactClass;
  let makeStyled:
    (~base: base, ~rule: rule('props, 'style), ~props: propsObject('props), 'children) =>
    statelessComponent;
  let makeStyledComponent:
    (
      ~rule: rule('props, 'style),
      ~component: statelessComponent,
      ~make: (~className: string, 'children) => statelessComponent,
      'children
    ) =>
    statelessComponent;
  module Provider: {let make: (~styletron: Core.t=?, 'children) => statelessComponent;};
};
