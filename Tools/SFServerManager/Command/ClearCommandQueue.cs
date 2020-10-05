////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2016 Kyungkun Ko
// 
// Author : KyungKun Ko
//
// Description : Clear command queue command
//
////////////////////////////////////////////////////////////////////////////////


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel.Composition;

using Sce.Atf;
using Sce.Atf.Adaptation;
using Sce.Atf.Applications;
using Sce.Atf.Dom;

using SF;
using System.IO;
using System.Runtime.Serialization.Json;

namespace SFServerManager.Command
{
    /// <summary>
    /// Upload XML server config to Zookeeper
    /// </summary>
    [Export(typeof(ClearCommandQueue))]
    [Export(typeof(IContextMenuCommandProvider))]
    [Export(typeof(IInitializable))]
    [PartCreationPolicy(CreationPolicy.Shared)]
    class ClearCommandQueue : ICommandClient, IInitializable, IContextMenuCommandProvider
    {

        public static CommandInfo[] MyCommands = {
            new CommandInfo(
                    "ClearCommandQueues".GetHashCode(),
                    StandardMenu.Edit,
                    StandardCommandGroup.EditGroup,
                    "Clear command queue".Localize(),
                    "Clear Zookeeper command queue".Localize(),
                    Sce.Atf.Input.Keys.None,
                    Resources.DeleteImage,
                    CommandVisibility.Default),
        };


        /// <summary>
        /// Constructor</summary>
        /// <param name="commandService">Command service</param>
        [ImportingConstructor]
        public ClearCommandQueue(ICommandService commandService, IContextRegistry contextRegistry)
        {
            m_commandService = commandService;
            m_contextRegistry = contextRegistry;
        }


        #region IInitializable Members

        void IInitializable.Initialize()
        {
            foreach (var myCommand in MyCommands)
                m_commandService.RegisterCommand(myCommand, this);
        }

        #endregion

        #region ICommandClient Members

        /// <summary>
        /// Checks whether the client can do the command, if it handles it</summary>
        /// <param name="commandTag">Command to be done</param>
        /// <returns>True iff client can do the command</returns>
        bool ICommandClient.CanDoCommand(object commandTag)
        {
            CommandInfo myCommand = null;
            foreach (var itCommand in MyCommands)
                if (itCommand.CommandTag.Equals(commandTag))
                {
                    myCommand = itCommand;
                    break;
                }
            if (myCommand == null)
                return false;

            if (m_Setting == null)
                return false;

            if (m_ZookeeperSession == null || m_ZookeeperSession.ZKInstance == null || !m_ZookeeperSession.ZKInstance.IsConnected())
                return false;

            if (string.IsNullOrEmpty(m_Setting.CommandNodePath))
                return false;


            return true;
        }

        /// <summary>
        /// Does the command</summary>
        /// <param name="commandTag">Command to be done</param>
        void ICommandClient.DoCommand(object commandTag)
        {
            CommandInfo myCommand = null;
            foreach (var itCommand in MyCommands)
                if (itCommand.CommandTag.Equals(commandTag))
                {
                    myCommand = itCommand;
                    break;
                }
            if (myCommand == null)
                return;

            if (m_ZookeeperSession == null || m_ZookeeperSession.ZKInstance == null || !m_ZookeeperSession.ZKInstance.IsConnected())
                return;

            if (string.IsNullOrEmpty(m_Setting.CommandNodePath))
                return;

            if (!myCommand.CommandTag.Equals("ClearCommandQueues".GetHashCode()))
                return;

            Outputs.WriteLine(OutputMessageType.Info, "Commencing Clear Command Queue");

            SFZookeeper zookeeper = m_ZookeeperSession.ZKInstance;
            if (zookeeper == null || !zookeeper.IsConnected())
            {
                Outputs.WriteLine(OutputMessageType.Error, "Zookeeper is not connected");
                return;
            }

            // Create command root node if not exists
            if (!zookeeper.Exists(m_Setting.CommandNodePath))
                zookeeper.CreateNode(m_Setting.CommandNodePath, "");


            if (!zookeeper.Exists(m_Setting.CommandNodePath))
                zookeeper.CreateNode(m_Setting.CommandNodePath, "");
            else
                zookeeper.DeleteAllChildren(m_Setting.CommandNodePath);

            Outputs.WriteLine(OutputMessageType.Info, "Command push process is done");
        }


        /// <summary>
        /// Updates command state for given command</summary>
        /// <param name="commandTag">Command</param>
        /// <param name="commandState">Command info to update</param>
        void ICommandClient.UpdateCommand(object commandTag, CommandState commandState)
        {
        }

        #endregion


        #region IContextMenuCommandProvider Members

        /// <summary>
        /// Gets tags for context menu (right click) commands</summary>
        /// <param name="context">Context containing target object</param>
        /// <param name="clicked">Right clicked object, or null if none</param>
        IEnumerable<object> IContextMenuCommandProvider.GetCommands(object context, object clicked)
        {
            ISelectionContext selectionContext = context.As<ISelectionContext>();
            if (selectionContext == null)
                return EmptyEnumerable<object>.Instance;

            return new object[]
                {
                    MyCommands[0].CommandTag,
                };
        }

        #endregion




        [Import(AllowDefault = false)]
        private ZookeeperSession m_ZookeeperSession = null;

        [Import(AllowDefault = false)]
        private ServerManagerSetting m_Setting = null;

        private readonly ICommandService m_commandService;
        private readonly IContextRegistry m_contextRegistry;
    }
}
